#!/usr/bin/env python3
# ──────────────────────────────────────────────────────────────────────────────
#  Imports
# ──────────────────────────────────────────────────────────────────────────────
import argparse
import hashlib
import os
import shutil

import networkx as nx
import pydot
import torch
import torch.nn.functional as F
from torch.nn import Linear
from torch_geometric.nn import GCNConv, NNConv, global_mean_pool

from utils import predict_from_graph
# ──────────────────────────────────────────────────────────────────────────────


# ──────────────────────────────────────────────────────────────────────────────
#  Graph‑handling helpers
# ──────────────────────────────────────────────────────────────────────────────
def _compress_graph_path(long_path: str) -> str:
    """Return same dir, but basename = md5(long_path).ext (32 chars)."""
    dir_, fname = os.path.split(long_path)
    _, ext = os.path.splitext(fname)
    digest = hashlib.md5(long_path.encode()).hexdigest()
    return os.path.join(dir_, f"{digest}{ext}")


def resolve_dot_path(long_path: str) -> str:
    """
    • If the hashed file already exists → return it.
    • Else if the *original* exists and can be accessed, copy → hashed and
      return the hashed name (preferred, keeps future opens short).
    • Else, if the original exists but copy fails (rare), return original.
    • Otherwise raise FileNotFoundError.
    """
    safe_path = _compress_graph_path(long_path)

    # 1. Hashed file already on disk → use it.
    if os.path.exists(safe_path):
        return safe_path

    # 2. Original exists?  Try to copy to hashed name for future runs.
    if os.path.exists(long_path):
        try:
            os.makedirs(os.path.dirname(safe_path), exist_ok=True)
            shutil.copy2(long_path, safe_path)      # may raise OSError
            return safe_path
        except OSError as e:
            print("long_path cannot be copied: ", e)
            # Could not copy (permissions, fs limit, …) – fall back.
            return long_path

    # 3. Nothing found.
    raise FileNotFoundError(
        f"DOT file missing.\nTried:\n  {safe_path}\n  {long_path}\n"
        "Make sure your generator saves the graph under one of these names."
    )


def load_nx_graph(dot_path: str) -> nx.DiGraph:
    """Parse a Graphviz .dot file → NetworkX DiGraph."""
    pydot_graph, = pydot.graph_from_dot_file(dot_path)
    return nx.DiGraph(nx.nx_pydot.from_pydot(pydot_graph))


# ──────────────────────────────────────────────────────────────────────────────
#  GNN definition
# ──────────────────────────────────────────────────────────────────────────────
class GNN(torch.nn.Module):
    def __init__(self, node_input_dim, edge_input_dim, hidden_dim=64,
                 use_edge_attr=False):
        super().__init__()
        self.use_edge_attr = use_edge_attr

        if self.use_edge_attr:
            # NNConv layer 1
            self.edge_mlp1 = torch.nn.Sequential(
                torch.nn.Linear(edge_input_dim, hidden_dim),
                torch.nn.ReLU(),
                torch.nn.Linear(hidden_dim,
                                node_input_dim * hidden_dim)  # 2 × 64 = 128
            )
            self.conv1 = NNConv(node_input_dim, hidden_dim,
                                nn=self.edge_mlp1, aggr="mean")

            # NNConv layer 2
            self.edge_mlp2 = torch.nn.Sequential(
                torch.nn.Linear(edge_input_dim, hidden_dim),
                torch.nn.ReLU(),
                torch.nn.Linear(hidden_dim,
                                hidden_dim * hidden_dim)  # 64 × 64 = 4096
            )
            self.conv2 = NNConv(hidden_dim, hidden_dim,
                                nn=self.edge_mlp2, aggr="mean")
        else:
            self.conv1 = GCNConv(node_input_dim, hidden_dim)
            self.conv2 = GCNConv(hidden_dim, hidden_dim)

        self.lin = Linear(hidden_dim, 1)  # scalar output

    def forward(self, x, edge_index, edge_attr, batch):
        if self.use_edge_attr:
            x = self.conv1(x, edge_index, edge_attr)
        else:
            x = self.conv1(x, edge_index)
        x = F.relu(x)

        if self.use_edge_attr:
            x = self.conv2(x, edge_index, edge_attr)
        else:
            x = self.conv2(x, edge_index)

        x = global_mean_pool(x, batch)
        return self.lin(x).squeeze(-1)


# ──────────────────────────────────────────────────────────────────────────────
#  Main CLI entry‑point
# ──────────────────────────────────────────────────────────────────────────────
def main_prediction() -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    parser = argparse.ArgumentParser(
        description="Run GNN heuristic prediction on a DOT graph.")
    parser.add_argument("path",     type=str, help="Path (long) to graph file")
    parser.add_argument("depth",    type=int, help="Depth parameter")
    parser.add_argument("n_agents", type=int, help="Number of agents")
    args = parser.parse_args()

    # 1) Resolve filename safely
    dot_path = resolve_dot_path(args.path)

    # 2) Load graph
    graph = load_nx_graph(dot_path)

    # 3) Build / load model
    subdir     = args.path.split("out/state/")[1].split('/')[0]
    model_dir  = os.path.join("lib", "RL", "results", subdir)
    model_file = os.path.join(model_dir, "gnn_predictor.pt")

    model = GNN(node_input_dim=2,
                edge_input_dim=args.n_agents,
                hidden_dim=64,
                use_edge_attr=True).to(device)

    model.load_state_dict(torch.load(model_file, map_location=device))

    # 4) Predict
    pred = predict_from_graph(model, graph, args.depth, device)

    # 5) Output
    with open("prediction.tmp", "w", encoding="utf-8") as fp:
        fp.write(f"VALUE:{pred}\n")


if __name__ == "__main__":
    main_prediction()
