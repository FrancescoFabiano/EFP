#script (python)
from clingo import Function
import time

def get(val, default):
    return val if val != None else default

def main(prg):

    total_solving = 0;
    total_grounding = 0;

    imin  = get(prg.get_const("imin"), 1)
    imax  = prg.get_const("imax")
    istop = get(prg.get_const("istop"), "SAT")

    step, ret = 0, None
    while ((imax is None or step < imax) and
            (step == 0 or step < imin or (
                (istop == "SAT"     and not ret.satisfiable  ) or
                (istop == "UNSAT"   and not ret.unsatisfiable) or
                (istop == "UNKNOWN" and not ret.unknown)))):
        parts = []
        parts.append(("check", [step]))

        if step > 0:
            tic_grounding = time.process_time();
            prg.release_external(Function("query", [step-1]))
            parts.append(("step" , [step]))
            prg.cleanup()
            toc_grounding = time.process_time();
        else:
            tic_grounding = time.process_time();
            parts.append(("base", []))
            toc_grounding = time.process_time();

        partial_grounding = toc_grounding - tic_grounding;

        tic_grounding = time.perf_counter();
        prg.ground(parts);
        prg.assign_external(Function("query", [step]), True);
        toc_grounding = time.perf_counter();
        partial_grounding += toc_grounding-tic_grounding
        print(f"i-th Grounding time is {partial_grounding} seconds")

        tic_solving = time.perf_counter();
        ret = prg.solve();
        toc_solving = time.perf_counter();
        print(f"i-th Solving time is {toc_solving-tic_solving} seconds")

        step = step+1;
        total_grounding += partial_grounding;
        total_solving += toc_solving - tic_solving;
    print(f"Total Grounding time is {total_grounding} seconds")
    print(f"Total Solving time is {total_solving} seconds")

#end.

#program check(t).
#external query(t).
