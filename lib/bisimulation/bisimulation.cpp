#include "bisimulation.h"

void bisimulation::FillStructures(automa *A)
{
	/*
	    Questa funzione si occupa della trasformazione di un automa con i soli archi etichettati
	    in uno con i soli stati etichettati e dell'inizializzazione delle strutture dati X e Q
	    che vengono utilizzate dalle procedure che eseguono gli algoritmi di PaigeTarjan e di
	    FastBisimulation.
	    La chiamata a "fill_structures" verra' effettuata alla fine di "LoadFromFC2", che
	    memorizza in una struttura dati di tipo "automa" il grafo rappresentato all'interno del
	    file .FC2 passatogli come input. Tale struttura dati verra' passata come argomento a
	    "fill_structure" che la analizzera' e creera' un grafo equivalente con i soli stati
	    etichettati. La struttura dati in cui tale automa viene memorizzato e' G, anch'essa
	    utilizzata dalle procedure che implementano PaigeTarjan e FastBisimulation.

	    INPUT:      A, l'automa con soli archi etichettati che dev'essere convertito

	    OUTPUT:     nessuno (Gli array X, Q e G sono globali, quindi non e' stato necessario
			ritornare alcunche' come output)
	 */

	// Variabili
	BIS_indexType i;

	// Inizializzazione della struttura dati X. Tale struttura dati avra' dimensione iniziale
	// pari al numero totale di etichette presenti nell'automa. Ogni elemento dell'array X ha
	// un "puntatore" all'elemento precedente ed uno a quello successivo. L'elemento di indice
	// 0 non avra' alcun elemento precedente e cio' verra' indicato dal valore BIS_NIL, cosi' come
	// l'elemento di indice "A->Nbehavs" non avra' alcun elemento successivo.
	// Il campo "firstBlock" viene qui inizializzato a BIS_NIL, ma verra' riconsiderato e aggiornato
	// correttamente dalla funzione "setpointers".
	//std::cerr << "\nDEBUG: Started X\n" << std::flush;

	// Inizializzazione del primo e dell'ultimo blocco di X
	X[0].prevXBlock = BIS_NIL;
	X[0].nextXBlock = 1;
	X[0].firstBlock = BIS_NIL;
	X[A->Nbehavs - 1].prevXBlock = A->Nbehavs - 2;
	X[A->Nbehavs - 1].nextXBlock = BIS_NIL;
	X[A->Nbehavs - 1].firstBlock = BIS_NIL;



	// Inizializzazione dei restanti blocchi di X
	for (i = 1; i < (A->Nbehavs - 1); i++) {
		X[i].nextXBlock = i + 1;
		X[i].prevXBlock = i - 1;
		X[i].firstBlock = BIS_NIL;
	};
	//std::cerr << "\nDEBUG: Done X\n" << std::flush;

	// Questa funzione si occupa effettivamente della conversione del grafo "A", con i soli
	// archi etichettati, in uno con i soli stati etichettati. Inoltre assegna ad ogni stato
	// l'elemento di X che rappresenta la sua etichetta (per i dettagli si vedano le spiegazioni
	// delle funzioni "createG" e "setpointers")

	//std::cerr << "\nDEBUG: Create G\n" << std::flush;

	CreateG(A->Nvertex, A->Vertex);

	//std::cerr << "\nDEBUG: Create G Done\n" << std::flush;

	// Gestione dei puntatori tra le strutture dati G ed X

	//std::cerr << "\nDEBUG: Pointers\n" << std::flush;

	SetPointers(A->Nbehavs);

	//std::cerr << "\nDEBUG: Pointers Done\n" << std::flush;


	// Procedura che inizializza i restanti campi di G (quelli non inizializzati dalle precedenti
	// funzioni) e quelli di Q
}

void bisimulation::DisposeAutoma(automa *A)
/* Rilascia la memoria allocata per l'automa */
{
	BIS_indexType i, j, Nv, Ne;
	v_elem *Vertex;

	Nv = A->Nvertex;
	Vertex = A->Vertex;

	for (i = 0; i < Nv; i++) {
		Ne = Vertex[i].ne;
		for (j = 0; j < Ne; j++) free(Vertex[i].e[j].bh);
		free(Vertex[i].e);
	}
	free(A);
}

void bisimulation::CreateG(int num_v, v_elem *Gtemp)
{
	/*
	    Questa funzione si occupa effettivamente della conversione del grafo con soli archi
	    etichettati, passatogli come input, in uno con soli stati etichettati.
	    Stabilisce inoltre, per ogni stato, a quale blocco esso appartenga (per blocco si intende
	    un elemento dell'array X, che rappresenta in modo univoco una etichetta del grafo). In altre
	    parole, all'inizio, tutti gli stati con uguale etichetta apparterranno ad uno stesso blocco.
	    L'array X ha dimensione pari al numero totale delle etichette.

	    INPUT:      num_v, il numero di vertici del grafo Gtemp
			Gtemp, il grafo con i soli archi etichettati

	    OUTPUT:     nessuno (L'array G e' globale, quindi non e' stato necessario ritornare
			alcunche' come output)
	 */

	// Variabili
	BIS_indexType v, e, b;
	struct adjList **curr_adj;

	// Creo "num_v" vertici in G. Questi sono gli stati presenti anche nel grafo con soli archi
	// etichettati Gtemp e che, per convenzione implementativa, avranno tutti etichetta 0 (in
	// realta' la loro etichetta potrebbe assumere un qualunque valore). Tutti questi stati
	// apparterranno quindi, come si puo' notare dall'assegnamento "G[v].block = 0", al blocco
	// 0, ovvero all'elemento dell'array X di indice 0.
	int n_id, n_agents = domain::get_instance().get_agents().size();


	//std::cerr << "\nDEBUG: For\n" << std::flush;

	//	std::vector<kworld_ptr>::const_iterator it_kwp;
	//	int temp_counter = 0;
	//	for (it_kwp = m_kworld_vec.begin(); it_kwp != m_kworld_vec.end(); it_kwp++) {
	//		std::cerr << "DEBUG: World " << temp_counter << " has ID: " << it_kwp->get_numerical_id() << std::endl;
	//		temp_counter++;
	//	}


	for (v = 0; v < num_v; v++) {
		n_id = m_kworld_vec[v].get_numerical_id();
		G[v].block = m_compact_indices[n_id];
		G[v].label = n_id + n_agents; // We reserve the values from 0 to n-1 to the labels of agents nodes


		//		n_id = 0;
		//		G[v].block = m_compact_indices[n_id];
		//		G[v].label = n_id + n_agents; // We reserve the values from 0 to n-1 to the labels of agents nodes
	}
	//std::cerr << "\nDEBUG: For Done\n" << std::flush;

	// La variabile "numberOfNodes" e' globale e indica il numero totale di stati del grafo su
	// cui verranno eseguiti gli algorimi di PaigeTarjan e FastBisimulation. A questo punto sono
	// stati creati "num_v" stati, pari al numero totale di stati del grafo con i soli archi
	// etichettati; il suo valore iniziale sara' quindi "num_v" e crescera' man mano che gli archi
	// vengono disetichettati.
	numberOfNodes = num_v;

	// Creo tutti gli altri stati (disetichettando gli archi). Per ogni stato del grafo di partenza
	// Gtemp, scandisco la sua lista di adiacenza. I casi che si possono presentare sono i seguenti:
	// 1. un arco con un'unica etichetta
	// 2. un arco con piu' etichette
	// Nel primo caso, "spezzo" in due l'arco ed inserisco tra i due uno stato con etichetta uguale
	// a quella posseduta dall'arco "spezzato". Nel secondo caso, supponendo che l'arco abbia "n"
	// etichette, creero' "n" stati, ognuno dei quali avra' una delle etichette possedute dall'arco.
	// Tali stati manterranno l'ordine in cui le etichette sono memorizzate sull'arco, nel senso che
	// se l'etichetta "i" precedeva l'etichetta "j" sull'arco, vi sara' un arco dallo stato con
	// etichetta "i" verso quello con etichetta "j". Ovviamente il primo degli "n" stati avra' un
	// arco entrante proveniente dallo stato "v" che stiamo considerando, mentre l'ultimo degli "n"
	// stati, avra' un arco uscente verso lo stato in cui giungeva l'arco che stiamo "spezzando".

	//std::cerr << "\nDEBUG: Adj For start\n" << std::flush;


	for (v = 0; v < num_v; v++) {
		// Puntatore alla lista di adiacenza dello stato corrente, che verra' creata dalle seguenti
		// linee di codice
		curr_adj = &(G[v].adj);

		//std::cerr << "\nDEBUG: " << num_v << "Adj For Internal start\n" << std::flush;


		//We consider Gtemp[v].ne as the total number of edges, in the original version ne is edges/nb
		e = 0;

		// Ciclo che considera tutti gli archi uscenti da "v"
		for (BIS_indexType e1 = 0; e1 < Gtemp[v].ne; e1++) {

			//Debug

			// Creo un nuovo stato (il primo nella catena di etichette). Come detto precedentemente
			// il campo "block" viene qui inizializzato con l'indice dell'elemento di X che rappresenta 
			// l'etichetta dello stato che stiamo creando
			//std::cerr << "\nDEBUG: Ini\n" << std::flush;
			//std::cerr << "\nDEBUG: numberOfNodes is: " << numberOfNodes << std::endl;
			//std::cerr << "\nDEBUG: e is: " << e << std::endl;
			//std::cerr << "\nDEBUG: Gtemp[v].ne is: " << Gtemp[v].ne << std::endl;
			//std::cerr << "\nDEBUG: v is: " << v << std::endl;
			//std::cerr << "\nDEBUG: label is: " << G[numberOfNodes].label << std::endl;


			G[numberOfNodes].block = Gtemp[v].e[e].bh[0];
			//std::cerr << "\n1" << std::endl;
			G[numberOfNodes].label = Gtemp[v].e[e].bh[0];
			//std::cerr << "\n2" << std::endl;

			numberOfNodes++;
			//	std::cerr << "\nDEBUG: numberOfNodes is: " << numberOfNodes << std::endl;
			//std::cerr << "\nDEBUG: Break\n" << std::flush;


			// Aggiornamento della lista di adiacenza di v. Viene aggiunto un nuovo elemento nella
			// lista di adiacenza di v, ovvero un nuovo arco che raggiunge lo stato appenza creato
			*curr_adj = new struct adjList;
			(*curr_adj)->node = numberOfNodes - 1;
			(*curr_adj)->next = NULL;
			curr_adj = &((*curr_adj)->next);
			//std::cerr << "\nDEBUG: Adj For Internal Internal\n" << std::flush;

			// Creo gli stati dal secondo in poi nella catena delle etichette. Questo ciclo viene
			// eseguito solamente nel caso in cui l'arco che stiamo considerando possegga piu' di
			// una etichetta
			for (b = 1; b < Gtemp[v].e[e].nbh; b++) { // **************************** TODO: CHECK IF LABELS AND BLOCKS ARE CORRECT!! ****************************
				// Creo un nuovo stato
				G[numberOfNodes].block = Gtemp[v].e[e].bh[b];
				G[numberOfNodes].label = Gtemp[v].e[e].bh[b];

				// Aggiorno la lista di adiacenza dello stato precedentemente creato, ovvero aggiungo
				// un arco dal penultimo stato che abbiamo creato a quello appena creato
				G[numberOfNodes - 1].adj = new struct adjList;
				G[numberOfNodes - 1].adj->node = numberOfNodes;
				G[numberOfNodes - 1].adj->next = NULL;

				numberOfNodes++;
				//Increment also for every bh the counter
				e1++;
			}

			//std::cerr << "\nDEBUG: Adj For Internal Internal Done\n" << std::flush;


			// Aggiornamento della lista di adiacenza dell'ultimo stato che abbiamo creato
			G[numberOfNodes - 1].adj = new struct adjList;
			G[numberOfNodes - 1].adj->node = Gtemp[v].e[e].tv;
			G[numberOfNodes - 1].adj->next = NULL;
			//std::cerr << "\nDEBUG: Here\n" << std::flush;

			e++;

		}

		//std::cerr << "\nDEBUG: Adj For Internal Done\n" << std::flush;

	}
	//std::cerr << "\nDEBUG: numberOfNodes is: " << numberOfNodes << std::endl;

	//std::cerr << "\nDEBUG: Adj For Done\n" << std::flush;

}

void bisimulation::SetPointers(int n)
{
	/*
	    Questa funzione gestisce i puntatori tra l'array X e l'array G nel seguente modo:
	    si e' detto che l'array X ha dimesione pari al numero totale di etichette presenti nel grafo.
	    Ogni elemento di questo array rappresenta quindi una classe di stati, ovvero tutti quelli che
	    hanno come etichetta quella di cui tale elemento e' il rappresentante (quindi tutti gli stati
	    con uguale etichetta avranno lo stesso campo "block"). Inoltre, ogni elemento dell'array X
	    possiede uno stato rappresentante (indicato dal campo "firstBlock" di X) ed ogni stato avra'
	    due "puntatori" al precedente ed al successivo elemento nel blocco. Cosicche', dato un
	    qualunque elemento (blocco) di X, guardando il campo "firstBlock" e seguendo i puntatori agli
	    elementi precedente e successivo, e' possibile individuare tutti gli stati appartenenti a tale
	    blocco.

	    INPUT:      n, il numero totale di etichette

	    OUTPUT:     nessuno (G ed X sono globali)
	 */

	// Variabili
	BIS_indexType *curr_node, i, x;

	// Allocazione dell'array temporaneo utilizzato dal ciclo seguente
	curr_node = new int[n];

	//
	for (i = 0; i < numberOfNodes; i++) {
		// Prelevo il blocco a cui appartiene lo stato "i"
		x = G[i].block;

		// Se tale blocco non ha ancora un rappresentante (il campo "firstBlock" e' uguale a BIS_NIL)
		// allora lo stato "i" diventa il suo rappresentante. Ovvero scelgo come rappresentante di
		// un blocco che rappresenta l'etichetta "k", il primo stato che trovo che abbia etichetta "k"
		if (X[x].firstBlock == BIS_NIL) {
			X[x].firstBlock = i;
			G[i].prevInBlock = BIS_NIL;
			G[i].nextInBlock = BIS_NIL;
		}// Se invece tale blocco "x" ha gia' un rappresentante, lego lo stato i, attraverso i puntatori
			// "prevInBlock" e "nextInBlock" all'ultimo elemento che ho trovato con uguale etichetta.
			// L'array "curr_node" viene appunto usato per tenere traccia di quale sia l'ultimo stato che
			// ho trovato, per ogni etichetta (curr_node ha dimensione pari al numero totale di etichette)
		else {
			G[i].prevInBlock = curr_node[x];
			G[i].nextInBlock = BIS_NIL;
			G[curr_node[x]].nextInBlock = i;
		};

		// Aggiorno l'array temporaneo
		curr_node[x] = i;
	};

	// Deallocazione dell'array temporaneo
	delete[] curr_node;
}

void bisimulation::GetMinimizedAutoma(automa *A)
{
	/*
	    Questa funzione crea, a partire dal grafo minimizzato (con soli stati etichettati) memorizzato
	    in G e dal grafo iniziale A (con soli archi etichettati) passato come input, il grafo minimizzato
	    con soli archi etichettati, che verra' salvato su un file .FC2 dalla funzione "SaveToFC2"

	    INPUT:      A, l'automa di partenza con i soli archi etichettati

	    OUTPUT:     nessuno
	 */

	// Con questa funzione vengono indicati gli stati che gli algoritmi di PaigeTarjan e
	// FastBisimulation hanno eliminato (in realta' tali algoritmi non operano tale eliminazione,
	// che viene effettivamente compiuta dalle funzioni "DeleteNodes" e "SaveToFC2")
	MarkDeletedNodes();

	// Viene effettuata la vera e propria eliminazione degli stati marcati com "BIS_DA_CANCELLARE" dalla
	// funzione "MarkDeletedNodes"
	DeleteNodes(A);
}

void bisimulation::MarkDeletedNodes()
{
	/*
	    MarkDeletedNodes non fa altro che marcare gli stati che devono essere cancellati, valutando
	    il valore che alcuni campi degli array G e Q assumono al termine della computazione delle
	    funzioni che implementano gli algoritmi di PaigeTarjan e FastBisimulation. Tali stati vengono
	    effettivamente eliminati dalle funzioni "DeleteNodes" e "SaveToFC2".

	    Nota: le implementazioni degli algoritmi di minimizzazione qui proposte sono distruttive
		  rispetto agli array Q ed X, pertanto l'unico modo per sapere quali stati sono bisimili
		  tra loro, e' controllare il campo "block" di tali stati, che indica dunque il blocco
		  cui essi appartengono.

	    INPUT:      nessuno

	    OUTPUT:     nessuno
	 */

	// Variabili
	BIS_indexType i, q;

	// Al termine delle funzioni di minimizzazione, alcuni campi di Q non servono piu' e vengono
	// qui riutilizzati per altri scopi. Il primo ad essere preso in considerazione e' il campo "size"
	// che viene usato per tenere traccia del fatto che un blocco di Q abbia o meno uno stato
	// rappresentante (un elemento (blocco) di Q rappresenta un insieme di stati bisimili)
	// Inizializzazione del campo "size" a BIS_NOTUSED, per indicare che nessun elemento (blocco) di Q ha
	// ancora il proprio stato rappresentante
	for (i = 0; i < QBlockLimit; i++) //il max di Q puo' essere superiore a nON *****************************************
		Q[i].size = BIS_NOTUSED;

	// Annoto gli stati che devono essere cancellati
	for (i = 0; i < numberOfNodes; i++) {
		// Considero il blocco cui appartiene lo stato "i"
		q = G[i].block;

		// Se tale blocco non ha ancora un rappresentante (il campo "size" e' uguale a BIS_NOTUSED)
		// allora lo stato "i" diventa il suo rappresentante. Ovvero scelgo come rappresentante di
		// un blocco "k", il primo stato che trovo che abbia il campo "block" con valore "k"
		if (Q[q].size == BIS_NOTUSED) {
			Q[q].size = BIS_USED;
			Q[q].firstNode = i;
		}// Altrimenti indico che tale stato dev'essere cancellato, in quanto bisimile allo stato
			// rappresentante del blocco cui esso appartiene. Utilizzo il campo "nextInBlock" in quanto
			// non sara' piu' utilizzato da alcuna funzione
		else
			G[i].nextInBlock = BIS_DA_CANCELLARE;
	}
}

void bisimulation::DeleteNodes(automa *A)
{
	/*
	    DeleteNodes si occupa di indicare quali tra gli stati del grafo di partenza A, passato come input,
	    devono essere cancellati, considerando il campo "nextInBlock" degli elementi dell'array G, modificato
	    dalla funzione "MarkDeletedNodes".
	    L'automa A che viene modificato da questa funzione verra' successivamente passato come input alla
	    funzione "SaveToFC2" che capira' quali stati e quali archi debbano essere memorizzati nel file .FC2
	    che produrra' come output.

	    INPUT:      A, l'automa di partenza, con i soli archi etichettati

	    OUTPUT:     nessuno
	 */

	// Variabili
	BIS_indexType i, j;

	// Segno nell'automa A di partenza quali tra i suoi stati sono stati indicati come "BIS_DA_CANCELLARE"
	// dalla funzione "MarkDeletedNodes"
	for (i = 0; i < A->Nvertex; i++) {
		// Se lo stato "i" dev'essere cancellato, ovvero se la funzione "MarkDeletedNodes" ha impostato
		// il suo campo "nextInBlock" a "BIS_DA_CANCELLARE", allora indico in A che tale nodo dev'essere
		// cancellato. Utilizzo il campo "ne" di "A->Vertex", che indica il numero di archi uscenti, in
		// quanto, visto che tale stato verra' cancellato, esso non servira' piu'
		if (G[i].nextInBlock == BIS_DA_CANCELLARE)
			A->Vertex[i].ne = BIS_DELETED;

			// Se invece tale stato dev'essere conservato, modifico gli archi (se necessario) come segue
		else {
			for (j = 0; j < A->Vertex[i].ne; j++) {
				// Modifica degli archi: se un nodo di quelli che non devono essere cancellati ha un
				// arco uscente che giunge ad un nodo marcato come "BIS_DA_CANCELLARE", faccio puntare
				// l'arco verso il nodo rappresentante del blocco a cui tale nodo da cancellare appartiene.
				// Utilizzo il campo "tv" di "A->Vertex[].e[]", che indica lo stato a cui punta l'arco
				// considerato, in quanto non servira' piu'
				if (G[ A->Vertex[i].e[j].tv ].nextInBlock == BIS_DA_CANCELLARE)
					A->Vertex[i].e[j].tv = Q[ G[A->Vertex[i].e[j].tv].block ].firstNode;
			}
		}
	}
}

void bisimulation::Inverse()
{
	BIS_indexType i;
	struct adjList *adj;
	struct adjList_1 *a;

	for (i = 0; i < numberOfNodes; i++) {
		adj = G[i].adj;
		while (adj != NULL) {
			adj->countxS = NULL; //initialisation to avoid inconsistent pointers
			a = new struct adjList_1;
			a->node = i;
			a->next = G[adj->node].adj_1;
			G[adj->node].adj_1 = a;
			a->adj = adj;
			adj = adj->next;
		}
	}
}

int bisimulation::InitPaigeTarjan()
{
	BIS_indexType i, l, end, temp;
	struct counter *cxS;
	struct adjList *adj;

	//initialisation of the graph (G,Q,X)
	for (l = 0; l != BIS_NIL; l = temp) { //for each label block
		temp = X[l].nextXBlock;
		if (temp == BIS_NIL)
			end = l;
		Q[l].prevBlock = X[l].prevXBlock;
		Q[l].nextBlock = X[l].nextXBlock;
		Q[l].firstNode = X[l].firstBlock;
		Q[l].superBlock = 0;
		//compute Q[].size
		Q[l].size = 0;
		for (i = X[l].firstBlock; i != BIS_NIL; i = G[i].nextInBlock)//for each node
			(Q[l].size)++;
		X[l].prevXBlock = BIS_NIL;
		X[l].firstBlock = BIS_NIL;
		X[l].nextXBlock = l + 1;

		B1[l] = numberOfNodes;
		B_1[l] = numberOfNodes;
		B_1[l] = numberOfNodes;
		splitD[l] = numberOfNodes;
	}

	X[0].nextXBlock = BIS_NIL;
	X[0].prevXBlock = BIS_NIL;
	X[0].firstBlock = 0;

	if (end == numberOfNodes)
		freeQBlock = BIS_NIL;
	else
		freeQBlock = end + 1;
	QBlockLimit = numberOfNodes;
	freeXBlock = 1;

	for (i = end + 1; i < numberOfNodes; i++) {
		Q[i].size = 0;
		Q[i].nextBlock = i + 1;
		Q[i].superBlock = BIS_NIL;
		Q[i].prevBlock = BIS_NIL;
		Q[i].firstNode = BIS_NIL;

		X[i].nextXBlock = i + 1;
		X[i].prevXBlock = BIS_NIL;
		X[i].firstBlock = BIS_NIL;

		B1[i] = numberOfNodes;
		B_1[i] = numberOfNodes;
		B_1[i] = numberOfNodes;
		splitD[i] = numberOfNodes;
	}
	Q[numberOfNodes - 1].nextBlock = BIS_NIL;
	X[numberOfNodes - 1].nextXBlock = BIS_NIL;


	if (Q[0].nextBlock == BIS_NIL) //P&T not necessary
		return 1;

	C = 0;
	//initialisation of the counters
	//initially there is a count per node count(x,U)=|E({x})|
	for (i = 0; i < numberOfNodes; i++) {
		adj = G[i].adj;
		//to avoid the creation of a counter set to zero
		if (adj == NULL)
			continue;
		cxS = new struct counter;
		cxS->value = 0;
		while (adj != NULL) {
			(cxS->value)++;
			/*each edge xEy contains a pointer to count(x,U);
			remember that each edge y(E-1)x contains a pointer to the edge xEy!*/
			adj->countxS = cxS;
			adj = adj->next;
		}
	}
	return 0;
}
//compute Paige and Tarjan

void bisimulation::PaigeTarjan()
{
	BIS_indexType S, S1; //pointer to the X-Blocks S and S1
	BIS_indexType B, S_B; //pointer to the Q-Blocks B and S-B
	BIS_indexType oldD, newD; //old and new block of x belonging to E-1(B)
	struct adjList_1 *adj;
	struct counter *cxS;
	BIS_indexType x, y, d, e;

	//std::cerr << "\nDEBUG: WHILE START\n" << std::flush;


	while (C != BIS_NIL) {
		//std::cerr << "\nDEBUG: INI\n" << std::flush;

		/*Step 1(select a refining block) & Step 2(update X)*/
		//select some block S from C
		S = C;
		//std::cerr << "\nDEBUG: INI ASSIGNEMENT\n" << std::flush;

		/*if S has more than two blocks, it has to be put back to C;
		hence it is not removed from X until we are sure it is not still
		compound after removing B from it*/

		/*examine the first two blocks in the of blocks of Q contained in S;
		let B be the smaller, remove B from S*/
		//std::cerr << "First Block Size: " << Q[X[S].firstBlock].size << std::endl;
		std::cerr << "\nAccessing the blocks : " << Q[X[S].firstBlock].firstNode << " and " << Q[Q[X[S].firstBlock].nextBlock].firstNode << std::endl;
		//std::cerr << "Error in accessing next block of: " << Q[Q[X[S].firstBlock].nextBlock].size << std::endl;

		if (Q[X[S].firstBlock].size < Q[Q[X[S].firstBlock].nextBlock].size) {
			//std::cerr << "\nDEBUG: FIRST IF\n" << std::flush;

			B = X[S].firstBlock;
			S_B = Q[X[S].firstBlock].nextBlock;
			X[S].firstBlock = S_B;
			Q[B].nextBlock = BIS_NIL;
			Q[S_B].prevBlock = BIS_NIL;

		}
		else{

			//std::cerr << "\nDEBUG: FIRST ELSE\n" << std::flush;

			B = Q[X[S].firstBlock].nextBlock;
			S_B = X[S].firstBlock;
			Q[S_B].nextBlock = Q[B].nextBlock;
			if (Q[S_B].nextBlock != BIS_NIL)
				Q[Q[S_B].nextBlock].prevBlock = S_B;
			Q[B].nextBlock = BIS_NIL;
			Q[B].prevBlock = BIS_NIL;
		}
		std::cerr << "Accessed the blocks" << std::endl;

		//std::cerr << "\nDEBUG: FIRST IF ELSE DONE\n" << std::flush;


		//and create a new simple block S1 of X containing B as its only block of Q
		S1 = freeXBlock;
		freeXBlock = X[freeXBlock].nextXBlock;
		Q[B].superBlock = S1;
		X[S1].nextXBlock = BIS_NIL;
		//X[S1].prevXBlock = BIS_NIL;
		X[S1].firstBlock = B;
		//X[S1].countxS is initialised in step 3

		//check if S is still compound
		if (Q[S_B].nextBlock == BIS_NIL) {
			//not compound: remove S from C
			C = X[C].nextXBlock;
			if (C != BIS_NIL)
				X[C].prevXBlock = BIS_NIL;
			X[S].nextXBlock = BIS_NIL;
			//free the space of S as XBlock
			/*WE DO NOT FREE THE BLOCK S: the XBlock still exists but it is
			not in the chain of C*/
		}

		//std::cerr << "\nDEBUG: STEP 2 DONE\n" << std::flush;


		/*Step 3(compute E-1(B))*/
		/*by scanning the edges xEy such that y belongs to B
		and adding each element x in such an edge to E-1(B),
		if it has not already been added.
		Duplicates are suppressed by marking elements: B_1
		Side effect: copy the elements of B in B1
		During the same scan, compute count(x,B)=count(x,S1) because S1={B};
		create a new counter record and make G[x].countxB point to it*/
		y = b1List = Q[B].firstNode;
		b_1List = BIS_NIL;
		while (y != BIS_NIL) { //for each y belonging to B
			B1[y] = G[y].nextInBlock; //copy the elements of B in B1
			adj = G[y].adj_1;
			while (adj != NULL) { //for each node in the adj_1 of y
				x = adj->node;
				if (B_1[x] == numberOfNodes) {
					//node not already added to E-1(B)
					B_1[x] = b_1List;
					b_1List = x;
					//create a new counter: it is pointed by G[x].countxB    /*1*/
					cxS = new struct counter;
					cxS->node = x;
					cxS->value = 1;
					G[x].countxB = cxS; /*1*/
				} else
					(G[x].countxB->value)++;
				adj = adj->next; //next node in the adj_1 of y
			}
			//std::cerr << "\nDEBUG: STEP 3 2-WHILE\n" << std::flush;

			y = G[y].nextInBlock; //next node y belonging to B
		}
		//std::cerr << "\nDEBUG: STEP 3 1-WHILE\n" << std::flush;


		/*Step 4(refine Q with respect to B)*/
		/*for each block D of Q containing some element of E-1(B)
		split D into D1 = D ^ E-1(B) and D2 = D - D1*/
		dList = BIS_NIL;
		//do this by scanning the elements of E-1(B)
		x = b_1List;
		while (x != BIS_NIL) { //for each x belonging to E-1(B)
			//determine the block D of Q containing it
			oldD = G[x].block; //index of D (old block of x)
			//and create an associated block D1 if one does not already exist
			if (splitD[oldD] == numberOfNodes) {
				//block D not already split
				splitD[oldD] = dList;
				dList = oldD;
				//create a new block D1
				if (freeQBlock == BIS_NIL) { //check for free space in memory
					if (QBlockLimit == BIS_MAXINDEX) {
						std::cout << "memory limit";
						std::cin >> x;
						exit(-1);
					}
					freeQBlock = QBlockLimit++;
					Q[freeQBlock].size = 0;
					Q[freeQBlock].nextBlock = BIS_NIL;
					splitD[freeQBlock] = numberOfNodes;
					//not necessary to initialise
					//Q[freeQBlock].prevBlock = BIS_NIL;
					//Q[freeQBlock].superBlock = BIS_NIL;
					//Q[freeQBlock].firstNode = BIS_NIL;
				}
				newD = freeQBlock; //index of D1 (new block of x)
				freeQBlock = Q[freeQBlock].nextBlock;
				Q[newD].firstNode = BIS_NIL;
				/*insert D1 just after D, so we know that, if D has already been
				split, the associated D1 is the next block*/
				Q[newD].nextBlock = Q[oldD].nextBlock;
				Q[oldD].nextBlock = newD;
				Q[newD].prevBlock = oldD;
				if (Q[newD].nextBlock != BIS_NIL)
					Q[Q[newD].nextBlock].prevBlock = newD;
				Q[newD].superBlock = Q[oldD].superBlock;
			} else
				newD = Q[oldD].nextBlock;
			//move x from D to D1
			if (G[x].prevInBlock != BIS_NIL)
				G[G[x].prevInBlock].nextInBlock = G[x].nextInBlock;
			else
				Q[G[x].block].firstNode = G[x].nextInBlock;
			if (G[x].nextInBlock != BIS_NIL)
				G[G[x].nextInBlock].prevInBlock = G[x].prevInBlock;
			G[x].block = newD;
			G[x].nextInBlock = Q[newD].firstNode;
			G[x].prevInBlock = BIS_NIL;
			if (Q[newD].firstNode != BIS_NIL)
				G[Q[newD].firstNode].prevInBlock = x;
			Q[newD].firstNode = x;
			(Q[oldD].size)--;
			(Q[newD].size)++;

			y = x;
			x = B_1[x];
			//re-initialisation of B_1
			B_1[y] = numberOfNodes;
		}//endwhile

		//std::cerr << "\nDEBUG: STEP 4 1-WHILE DONE\n" << std::flush;

		//dList points to the list of new blocks splitD
		d = dList;
		while (d != BIS_NIL) {
			if (Q[d].firstNode == BIS_NIL) {
				//D empty: remove it and free its space
				if (Q[d].prevBlock != BIS_NIL)
					Q[Q[d].prevBlock].nextBlock = Q[d].nextBlock;
				else
					X[Q[d].superBlock].firstBlock = Q[d].nextBlock;
				//we are sure that after D,there is D1
				Q[Q[d].nextBlock].prevBlock = Q[d].prevBlock;
				//re-initialise Q[d]
				//Q[d].size is already zero
				Q[d].prevBlock = BIS_NIL;
				Q[d].superBlock = BIS_NIL;
				Q[d].firstNode = BIS_NIL;
				//free Q[d]
				Q[d].nextBlock = freeQBlock;
				freeQBlock = d;
			} else {
				/*if D nonempty and the superBlock containing D and D1 has been
				made compound by the split, add this block to C*/
				if (Q[d].prevBlock == BIS_NIL && Q[Q[d].nextBlock].nextBlock == BIS_NIL) {
					//D and D1 are the only blocks in this just split Xblock
					X[Q[d].superBlock].nextXBlock = C;
					X[Q[d].superBlock].prevXBlock = BIS_NIL;
					C = Q[d].superBlock;
					/*when D became the only block of an XBlock (see the end of step 2)
					we did not free its space and now we can re-use it!!*/
				}
			}

			e = d;
			d = splitD[d];
			//re-initialisation of splitD
			splitD[e] = numberOfNodes;
		}
		//std::cerr << "\nDEBUG: STEP 4 2-WHILE DONE\n" << std::flush;

		/*Step 5(compute E-1(B) - E-1(S_B))*/
		/*Scan each x such that xEy and y belongs to B; determine count(x,B)
		to which G[x].countxB points and count(x,S) to which xEy points
		(y belongs to B -> scan y(E-1)x -> y(E-1)x points to xEy ->
		  xEy points to count(x,S))
		To save space we use again the array B_1 to store E-1(B) - E-1(S - B)*/
		y = b1List;
		b_1List = BIS_NIL;
		while (y != BIS_NIL) { //for each y belonging to B1
			adj = G[y].adj_1;
			while (adj != NULL) { //for each node in the adj_1 of y -> scan xEy, y in B
				x = adj->node;
				if (G[x].countxB->value == adj->adj->countxS->value)
					if (B_1[x] == numberOfNodes) {
						//x is a node not already added to E-1(S - B)
						B_1[x] = b_1List;
						b_1List = x;
					}
				adj = adj->next;
			}
			y = B1[y];
		}
		//std::cerr << "\nDEBUG: STEP 5 DONE\n" << std::flush;

		/*Step 6(refine Q with respect to S_B)*/
		/*proceed exactly as in Step 4, but scan E-1(B) - E_1(S - B)
		For each block D of Q containing some element of E-1(B) - E-1(S - B)
		split D into D1 = D ^ (E-1(B) - E-1(S - B)) and D2 = D - D1*/
		dList = BIS_NIL;
		//do this by scanning the elements of E-1(B) - E-1(S - B)
		x = b_1List;
		while (x != BIS_NIL) {
			//to process an element x belonging to E-1(B) - E-1(S - B)
			//determine the block D of Q containing it
			oldD = G[x].block; //index of D (old block of x)
			//and create an associated block D1 if one does not already exist
			if (splitD[oldD] == numberOfNodes) {
				//block D not already split
				splitD[oldD] = dList;
				dList = oldD;
				//create a new block D1
				if (freeQBlock == BIS_NIL) { //check for free space in memory
					if (QBlockLimit == BIS_MAXINDEX) {
						std::cout << "memory limit";
						std::cin >> x;
						exit(-1);
					}
					freeQBlock = QBlockLimit++;
					Q[freeQBlock].size = 0;
					Q[freeQBlock].nextBlock = BIS_NIL;
					splitD[freeQBlock] = numberOfNodes;
					//not necessary to initialise
					//Q[freeQBlock].prevBlock = BIS_NIL;
					//Q[freeQBlock].superBlock = BIS_NIL;
					//Q[freeQBlock].firstNode = BIS_NIL;
				}
				newD = freeQBlock; //index of D1 (new block of x)
				freeQBlock = Q[freeQBlock].nextBlock;
				Q[newD].firstNode = BIS_NIL;
				/*insert D1 just after D, so we know that, if D has already
				been split, the associated D1 is the next block*/
				Q[newD].nextBlock = Q[oldD].nextBlock;
				Q[oldD].nextBlock = newD;
				Q[newD].prevBlock = oldD;
				if (Q[newD].nextBlock != BIS_NIL)
					Q[Q[newD].nextBlock].prevBlock = newD;
				Q[newD].superBlock = Q[oldD].superBlock;
			} else
				newD = Q[oldD].nextBlock;
			//move x from D to D1
			if (G[x].prevInBlock != BIS_NIL)
				G[G[x].prevInBlock].nextInBlock = G[x].nextInBlock;
			else
				Q[G[x].block].firstNode = G[x].nextInBlock;
			if (G[x].nextInBlock != BIS_NIL)
				G[G[x].nextInBlock].prevInBlock = G[x].prevInBlock;
			G[x].block = newD;
			G[x].nextInBlock = Q[newD].firstNode;
			G[x].prevInBlock = BIS_NIL;
			if (Q[newD].firstNode != BIS_NIL)
				G[Q[newD].firstNode].prevInBlock = x;
			Q[newD].firstNode = x;
			(Q[oldD].size)--;
			(Q[newD].size)++;

			y = x;
			x = B_1[x];
			//re-initialisation of B_1
			B_1[y] = numberOfNodes;
		}//endwhile
		//std::cerr << "\nDEBUG: STEP 6 1-WHILE DONE\n" << std::flush;

		//dList points to the list of new blocks splitD
		d = dList;
		while (d != BIS_NIL) {
			if (Q[d].firstNode == BIS_NIL) {
				//D empty: remove it and free its space
				if (Q[d].prevBlock != BIS_NIL)
					Q[Q[d].prevBlock].nextBlock = Q[d].nextBlock;
				else
					X[Q[d].superBlock].firstBlock = Q[d].nextBlock;
				Q[Q[d].nextBlock].prevBlock = Q[d].prevBlock;
				//re-initialise Q[d]
				//Q[d].size is already zero
				Q[d].prevBlock = BIS_NIL;
				Q[d].superBlock = BIS_NIL;
				Q[d].firstNode = BIS_NIL;
				//free Q[d]
				Q[d].nextBlock = freeQBlock;
				freeQBlock = d;
			} else {
				/*if D nonempty and the superBlock containing D and D1 has been
				made compound by the split, add this block to C*/
				if (Q[d].prevBlock == BIS_NIL && Q[Q[d].nextBlock].nextBlock == BIS_NIL) {
					//D and D1 are the only blocks in this just split Xblock
					X[Q[d].superBlock].nextXBlock = C;
					X[Q[d].superBlock].prevXBlock = BIS_NIL;
					C = Q[d].superBlock;
					/*when D became the only block of an XBlock (see the end of step 2)
					we did not free its space and now we can re-use it!!*/
				}
			}
			e = d;
			d = splitD[d];
			//re-initialisation of splitD
			splitD[e] = numberOfNodes;
		}

		////std::cerr << "\nDEBUG: STEP 6 2-WHILE DONE\n" << std::flush;


		/*Step 7(update counts)*/
		/*scan the edges xEy tc y belongs to B1.
		To process an edge decrement count(x,S) (to which xEy points).
		If this count becomes zero delete the count record,
		and make xEy point to count(x,B) (to which x points).
		Discard B1 (re-initialise it).*/
		y = b1List;

		while (y != BIS_NIL) { //for each y belonging to B

			adj = G[y].adj_1;

			while (adj != NULL) {//for each node in the adj_1 of y -> scan xEy, y in B
				x = adj->node;
				cxS = adj->adj->countxS;
				if (cxS->value != 1) {
					(cxS->value)--;
					adj->adj->countxS = G[x].countxB;
				} else { //count(x,S) becomes zero
					//make xEy point to count(x,B)
					adj->adj->countxS = G[x].countxB;
					//delete count(x,S)
					delete cxS;
				}
				adj = adj->next;
			}
			x = y;
			y = B1[y];
			//re-initialisation of B1
			B1[x] = numberOfNodes;
		}
		//std::cerr << "\nDEBUG: STEP 7 DONE\n" << std::flush;

	}//end while


}

bool bisimulation::MinimizeAutoma(automa *A)
{
	//std::cerr << "\nDEBUG: IN MINIMIZE\n" << std::flush;
	FillStructures(A);
	//std::cerr << "\nDEBUG: Filled structures\n" << std::flush;

	Inverse();

	//std::cerr << "\nDEBUG: Inverse done\n" << std::flush;


	if (InitPaigeTarjan() == 0) {
		//std::cerr << "\nDEBUG: IN PAIGE-TARJAN\n" << std::flush;

		PaigeTarjan();

		//std::cerr << "\nDEBUG: PAIGE-TARJAN DONE\n" << std::flush;


		/*CHECK BECAUSE THEY DON'T CONSIDER NODES WITH LABELS*/
		GetMinimizedAutoma(A);
		return true;

	}
	return false;

}
//

bisimulation::bisimulation(const std::map<kworld_ptr, int> & index_map, const std::vector<kworld_ptr> & kworld_vec, const std::map<int, int> & compact_indices)
{

	G = (graph *) malloc(sizeof(graph) * BIS_MAXINDEX);
	Q = (qPartition *) malloc(sizeof(qPartition) * BIS_MAXINDEX);
	X = (xPartition *) malloc(sizeof(xPartition) * BIS_MAXINDEX);

	m_index_map = index_map;
	m_kworld_vec = kworld_vec;
	m_compact_indices = compact_indices;
}

//
//bisimulation::bisimulation(){
//        struct graph G[BIS_MAXINDEX];
//        struct qPartition Q[BIS_MAXINDEX];
//        struct xPartition X[BIS_MAXINDEX];
//	
//}