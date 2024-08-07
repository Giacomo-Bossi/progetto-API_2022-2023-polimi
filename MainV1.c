#include <stdio.h>      //non uso RB ma albero standard
#include <stdlib.h>

typedef struct node{
    int pos;
    struct node* p;
    struct node* r;
    struct node* l;
    int* hash;
    int totspace;
    int usedspace;
    int max;
}sta;

typedef struct lists{
    int num;
    struct lists* next;
}list;

typedef struct lists2{
    sta* ptr;
    struct lists2* next;
}ptrlist;

sta* staroot;   /*radice dell albero stazioni -> riscrivo tutte le funzioni del RBtree*/
sta* Tnil;      /*foglia Tnil*/
list* percorso = NULL;
ptrlist* ptrhead = NULL;
int par1;
int par2;

/*FUNZIONI PER GESTIRE IL RBTREE*/
/*riguardare assolutamante sopratturtto per la cosa del T.nil invece che NULL*/
void inizializzaTNIL(){
    Tnil->l = Tnil;
    Tnil->r = Tnil;
    Tnil->p =Tnil;
    Tnil->hash = NULL;
    Tnil->max=0;
    Tnil->pos=0;
    Tnil->totspace=0;
    Tnil->usedspace=0;
    return ;
}

void initializeStation(int val, sta* station, sta* father) {        /*testata e funziona*/
    station->pos = val;
    station->p = father;
    station->r = Tnil;
    station->l = Tnil;
    station->hash= NULL;
    station->totspace = 0;
    station->usedspace = 0;
    station->max = 0;
    return;
}

void cpy(sta* dest, sta* source){       /*copia source su dest*/
    dest->pos = source->pos;
    dest->usedspace = source->usedspace;
    dest->totspace = source->totspace;
    dest->hash = source->hash;
    dest->max = source->max;
    return;
}

void leftrotate(sta* x){ /*restituisce puntatore alla nuova root se cambia altrimenti restituisce NULL*/
    sta*y;
    sta*father;
    sta*a;

    y=x->r;
    a = y->l;   /*a è l'elem che cambia padre*/
    x->r = a;
    if(a != Tnil)
        a->p = x;
    y->p = x->p;
    father = x->p;
    if(father == Tnil)    /*y è la nuova root*/
        staroot = y;
    else if (father->l == x)    /*cambio riferimento padre*/
        father->l = y;
    else father->r = y;

    y->l = x;
    x->p = y;
    return;
}

void rightrotate(sta* y){       /*rivedo meglio della left*/
    sta*x;
    sta*father;
    sta*a;

    x=y->l;
    a = x->r;   /*a è l'elem che cambia padre*/
    y->l = a;
    if(a != Tnil)
        a->p = y;
    x->p = y->p;
    father = y->p;
    if(father == Tnil)    /*y è la nuova root*/
        staroot = x;
    else if (father->l == y)    /*cambio riferimento padre*/
        father->l = x;
    else father->r = x;

    x->r = y;
    y->p = x;
    return;
}

sta* minRB(sta*root){   

    while(root->l != Tnil)
        root = root->l;
    
    return root;
}

sta* maxRB(sta*root){
    
    while(root->r != Tnil)
        root = root->r;
    
    return root;
}
sta* successor(sta* x){     /*ritorna Tnil se x è il massimo*/
    sta* y;       
    if(x->r != Tnil)
        return minRB(x->r);  /*minimo dei maggiori*/
    y = x->p;
    while(y!=Tnil && y->r == x){    /*cerca sopra*/
        x=y;
        y=y->p;
    }
    return y;
}

sta* predecessor(sta*x){    /*stesse cose di successor*/
    sta* y;
    if(x->l != Tnil)
        return maxRB(x->l);
    y=x->p;
    while(y!=Tnil && y->l == x){
        x=y;
        y=y->p;
    }
    return y;
}

sta* inserisciRB(int val){     /*restituisce il puntatore di deve è stato inserita la stazione per inserire le auto, NULL se stazione esiste già*/        
    sta* x;

    x = staroot;
    if(staroot == Tnil){     /*se è vuoto inserisco radice*/
        staroot = (sta*) malloc(sizeof(sta));
        initializeStation(val,staroot, Tnil);
        return staroot;
    }   
    else{   
        while(1){
            if(x->pos == val)
                return NULL;        
            if(val > x->pos){
                if(x->r == Tnil){
                    x->r = (sta*) malloc(sizeof(sta));
                    initializeStation(val, x->r, x);
                    return(x->r);
                }
                else x = x->r;
            }
            else{
                if(x->l == Tnil){
                    x->l = (sta*) malloc(sizeof(sta));
                    initializeStation(val, x->l, x);
                    return(x->l);
                }
                else x = x->l;
            }
        }
    }
}
sta* cercaRB(int val, sta* root){       /*dovrebbe andare*/ /*per uso staroot la prima chiamata ricorsiva chiamo carcaRB(val , staroot)*/

    if(root->pos == val || root == Tnil)        /*ho trovato oppure non c'è*/
        return root;

    if(val > root->pos)         /*cerco tra i più grandi*/
        return cercaRB(val ,root->r);
    else 
        return cercaRB(val ,root->l);      /*cerco tra i più piccoli*/        
}

void cancellaRB(sta* target){      /*per essere chiamata devo prima sapere se l'elem esiste e la sua posizione, questa funz. cancella e sistema l'albero*/  
    sta* dacanc;
    sta* sotto;
    sta* dacancp;
    if(target->r == Tnil || target->l == Tnil)      /*trova il nodo da cancellare effettivamente*/
        dacanc = target;
    else dacanc = successor(target);

    dacancp=dacanc->p;

    if(dacanc->l != Tnil)
        sotto = dacanc->l;
    else sotto = dacanc->r;    

    if(sotto != Tnil)               //controlla se sotto non è Tnil e sistema padre di sotto
        sotto->p = dacanc->p;
    if(dacanc->p == Tnil)           //sistema eventualmentela radice se tolgo l'ultimo nodo
        staroot = sotto;
    else if(dacanc == dacancp->l)
        dacancp->l = sotto;
    else dacancp->r = sotto;

    if(dacanc != target)
        cpy(target, dacanc);
    free((void*)dacanc);
    return;
}
/* FUNZIONI CHE GESTISCONO LA HASHMAP DELLE AUTO*/
/*convenzione -1 -> cella inutilizzata, -2 tombstone*/
int inserisciHash(int val, int hash[], int size){        /*testata e funziona*/ 
    int pos;
    int newpos;
    int i;
    pos= val % size;
    i=0;
    if(hash[pos] == -1 || hash[pos] == -2){   /*inserimento senza collisione*/
        hash[pos]=val;
        return 1;
    }
    else{       /*sequenza di ispezione*/
        newpos=pos;
        do
        { 
            i++;
            newpos=(newpos+i)%size;
        } while (hash[newpos]!=-1 && hash[newpos]!=-2 && newpos!=pos);      /*la sequanza passa da tutti i valori della tabella prima di ritoenare al valora iniziale pos (potenze 2)*/
                                                                            //in realtà non può tornare su pos perchè se è piena rialloco prima di inserire
        if(hash[newpos]==-1 || hash[newpos]==-2){   /*se ho trovato posizione*/
            hash[newpos]=val;
            return 1;
        }
        return 0;       /*ritorno 0 se non c'è spazio così il gestore può riallocare la Hash table e poi richiamare inserisciAuto una volta che ci sarà spazio*/
    }                   //non accadrà mai
}

int* riallocaHash(int Hash[], int size){        /*testata su array da 16 e funziona*/
    int* newHash;
    int newsize;
    int i;

    newsize=size * 2;  /*raddoppio la dimensione*/

    newHash = (int*) malloc(newsize*sizeof(int));   /*creo nuovo array*/

    for(i=0;i<newsize;i++)
        newHash[i]=-1;   /*inizializzo Hash nuovo*/

    for(i=0;i<size;i++)
        inserisciHash(Hash[i], newHash, newsize);       /*reinserisce i valori precedentemente inseriti*/

    free(Hash);     

    return newHash;
}

int cancellaDaHash(int val, int hash[], int size){            /*ricordarsi di chiamare cerca max se il vaolre cancellato è il max | ritorna 0 se non presente*/
    int pos;
    int newpos;
    int i;

    pos = val % size;
    if(hash[pos]==val){
        hash[pos]=-2; /*sostisuisco con tombstone*/
        return 1;
    }
    else{
        i=0;
        newpos = pos;
        do
        {
            i++;
            newpos=(newpos+i)%size;

        }while(hash[newpos]!=val && newpos!=pos);       //se non è presente passo tutte le celle

        if(hash[newpos] == val){
            hash[newpos] = -2;  /*cancella*/
            return 1;
        }
        else return 0;  /*il valore non è presente nella tabella*/
    }
}

int cercaMaxHash(int hash[], int size){         /*cerca il valore massimo e lo restituisce*/
    int max;
    int i;
    max=0;
    for(i=0; i<size; i++)
        if(hash[i]>max)
            max=hash[i];
    return max;     /*nota se la hash è vuota (solo celle vuote e tombstone) ritorno 0 e non i valori di tombstone/ cella vuota*/
}

int* creaHash(int size){       /*da chiamare solo alla creazione di una nuova stazione ( size dipende dall'' input iniziale)*/
    int *ptr;
    int i;

    ptr = (int*) malloc(size * sizeof(int));          /*da rivedere il numero di celle iniziale !!!deve rimanere una potenza di 2!!!*/

    for(i=0;i<size;i++)                       /*inizializzo a celle vuote*/                 
        ptr[i]=-1;
    return ptr;
}
/*FUNZIONI LISTE*/
/*lista puntatori risoluzione*/
void enqueuePtr(sta* ptr){   /*rivedo perchè ho cambiato il puntatore globale*/
    ptrlist* node;
    node = ptrhead;
    ptrhead = (ptrlist*) malloc(sizeof(ptrlist));
    ptrhead->next = node;
    ptrhead->ptr = ptr;

    return;
}

sta* dequeuePtr(){  /*estrae un puntatore dalla lista, se vuota ritorna NULL*/
    sta* ptr;
    ptrlist* node;

    if(ptrhead == NULL)
        return NULL;
    node = ptrhead->next;
    ptr= ptrhead->ptr;
    free(ptrhead);
    ptrhead=node;
    return ptr;
}
void clearPtrlist(){       /* svuota la lista in caso non ci sia percorso*/
    while(ptrhead != NULL)
        dequeuePtr();
    return; 
}
/*lista percorso*/
void enqueuePath(int val){     /*percorso fa la voce di head*/
    list* node;
    node = percorso;
    percorso = (list*) malloc(sizeof(list));
    percorso->num = val;
    percorso->next = node;
    return;
}
int dequeuePath(){
    int val;
    list* node;
    if(percorso == NULL)
        return -1;
    node = percorso->next;
    val = percorso->num;
    free(percorso);
    percorso=node;
    return val;    
}
void clearPath(){
    while(percorso != NULL)
        dequeuePath();
    return;
}

/*MAIN e funzioni principali*/
int aggiungistazione(int dist, int arr[], int len){                         //il ritorno 1 -> aggiunta, 0-> non aggiunta   tastato dopo inserim tutti i campi sono ok 
    int i;                                                                  // testato anche su input > 16 auto
    int space = 16;
    sta* station;

    station=inserisciRB(dist);      //non ancora riparato!!! (qui non uso RB)  inizializza già la stazione

    if(station == NULL)
        return 0;
    
    while(space < len)
        space = space * 2;      /* trova potenza di 2 più piccola che contiene input*/

    station->hash=creaHash(space);
    station->totspace=space;            /*all'inizio alloco space slot ed è vuoto */
    station->usedspace=len;             /*li metto subito dopo*/

    for(i=0; i<len; i++){                /*non può riempirsi perchè alloco spazio giusto*/
        inserisciHash(arr[i], station->hash, station->totspace);
        if(arr[i]>station->max)
            station->max = arr[i];
    }
    return 1;
}

int demoliscistazione(int dist){                     /*ritorno: 1-> demolita, 0-> non c'era*/            /*dovrebbe andare*/
    sta * station;

    station = cercaRB(dist, staroot);
    if(station == Tnil)     /*non c'è la stazione da cancellare*/
        return 0;
    else{
        free(station->hash);        /*dealloca la hash table*/
        cancellaRB(station);
    }
    return 1;
}

int aggiungiauto(int dist, int autonomy){            /*ritorno: 1-> aggiunta, 0-> la stazione non c'è*/      /*devrebbe andare*/
    sta* station;
    station = cercaRB(dist, staroot);  
    if(station == Tnil)
        return 0;
    else{
        station->usedspace++;
        if(station-> usedspace > station-> totspace){           /*caso tabella piena e quindi rialloco*/
            station->hash = riallocaHash(station->hash, station->totspace);
            station->totspace = station->totspace * 2;
        }
        inserisciHash(autonomy, station-> hash, station->totspace);
        if(autonomy >  station->max)    /*se aggiungo nuovo valore massimo*/
            station->max = autonomy;
    }
    return 1;
}

int rottamaAuto(int dist, int autonomy){             /*ritorno: 1-> rottamata 0->non c'è stazione o non c'è auto con tale autonomia*/        /*dovrebbe andare*/
    sta* station;
    int check;

    station = cercaRB(dist, staroot);
    if(station == Tnil)
        return 0;
    else{
        check = cancellaDaHash(autonomy,station->hash, station->totspace);
        if(check == 0)  /*non è presente la macchina da cancellare*/
            return 0;
        else if (autonomy == station->max){
                station->max = cercaMaxHash(station->hash, station->totspace);  /*se ho cancellato il massimo devo trovare il nuovo massimo*/
        }
        station->usedspace--;   //sistemo usedspace        
    }
    return 1;
}
char partizionaDX(int start, int arrive){       //non funziona sempre in modo corretto (crea troppi pochi paletti ) forse fixato
    sta* startptr;
    sta* curr;
    int stop;
    int far;

    startptr = cercaRB(start,staroot);
    curr = startptr;
    stop = curr->pos + curr->max;       //per primo passaggio
    far = stop;

    enqueuePtr(startptr);

    while(far < arrive){      

        curr = successor(curr);

        if(curr->pos > far){   //curr->pos non è ragguingibile da nessuno dei precedenti dato che far è il più lontano raggiungibile
            printf("nessun percorso\n");
            return 'n';
        }
        else{
            if(curr->pos > stop){      //incodo paletto e updato il prossimo stop
                enqueuePtr(curr);
                stop = far;
            }
            if(curr->pos + curr->max > far){    //update far
                far = curr->pos + curr->max;
                
                if(curr->pos + curr->max >= arrive){ //controllo se raggiunge arrive solo in caso di update di far
                    enqueuePath(curr->pos);
                    if(percorso->num == curr->pos)      //toglie eventualmente se il penultimo è stato incodato
                        dequeuePtr();
                }
            }
             
        }
    }
    if(curr->pos == start)
        return 't'; //trivial solution start->arrive  (non ho aggiunto nulla sulle code)
    return 's';
}

char partizionaSX(int start,int arrive){    //testare ma dovrebbe andare //da sistemare è trovapercorsoSX (loopa?)
    sta* startptr;
    sta* curr;
    int stop;
    int nextstop;

    startptr = cercaRB(start,staroot);
    curr = startptr;
    stop = curr->pos - curr->max;
    nextstop = stop;

    while(stop > arrive){
        
        curr = predecessor(curr);

        if(curr->pos < nextstop){       //vede se è raggiungibile
            printf("nessun percorso\n");
            return 'n';
        }
        else{
            if(curr->pos < stop){   //dovrebbe essere giusto
                enqueuePtr(curr);   //incodo il primo non raggiungibile con n passi           
                stop = nextstop;
                }
            if(curr->pos - curr->max < nextstop)        //update di nextstop
                    nextstop = curr->pos - curr->max;  
        }
    }
    if(curr->pos == start)
        return 't';     //trivial solution
    else{
        return 's';
    }
}

void trovapercorsoDX(int start){        //dovrebbe essere giusta
    sta*curr;
    
    curr=dequeuePtr(); //quello da cui inizio

    while(curr!=NULL){
        while(curr->pos + curr->max < percorso->num)
            curr = successor(curr);
        
        enqueuePath(curr->pos);
        curr = dequeuePtr();
    }
    if(percorso->num != start)
        enqueuePath(start);
    return;
}

void trovapercorsoSX(int arrive, int start){         //potrebbe andare, lo testo   
    sta* curr;
    curr=dequeuePtr();

    while(curr != NULL){ //ci arrivo quando faccio dequeue
        curr=successor(curr);
         if(curr->pos-curr->max <= arrive){
            enqueuePath(curr->pos);
            arrive=curr->pos;
            curr=dequeuePtr();  //passo al prossimo paletto
        }
    }
    if(percorso->num !=  start)
        enqueuePath(start);
    return;    
}
void stamparisultati(){     //le stampe mancata sono perchè non sono inserite non possono sfuggire a questa funzione
    int num;    
    num = dequeuePath();
    printf("%d",num);
    num = dequeuePath();
    while(num!= -1){
        printf(" %d",num);
        num = dequeuePath();       
    }
    printf("\n");
    return;
}

void pianificapercorso(int start, int arrive){
    char ok;    //le stazioni date sono sicuramente presenti
    
    if(start==arrive)
        printf("%d\n",start);
    else{
        clearPath();    //qualunque situazione precedente cancello le liste completamente
        clearPtrlist();
        enqueuePath(arrive);        /*l'ultimo sarà sempre l'arrivo*/

        if(start < arrive){     //mi muovo verso dx
            ok = partizionaDX(start,arrive);
            if(ok == 's'){
                trovapercorsoDX(start);
            }
        }
        else{                   /*mi muovo verso sx necessariamente*/
            ok = partizionaSX(start,arrive);
            if(ok == 's'){
                trovapercorsoSX(arrive, start);
            }
        }
        //parte della stampa
        if(ok == 't'){                          //soluzione trivial
            printf("%d %d\n",start, arrive);    
        }
        else if(ok == 's'){     //se il percorso minimo è non trivial
                stamparisultati();
        }
    }
    return;
}

char leggiriga(char str[]){ 
    int cestino;    /*serve solo ad evitare errore ignora return value di scanf*/
    
    if(scanf("%s",str) == -1){
        return 'n'; /*ho finito tutto l'input*/
    }
    cestino = scanf("%d", &par1);   
    if(str[0]== 'd')    /*se l'istruzione è demolisci ho un solo paramatro*/
        return 'd';
    cestino = scanf("%d", &par2);
    cestino++;              //per evitare variabile inutilizzata
    return str[11];  /*la lettera in quella pos identifica in modo univoco l' istruzione*/
}

int main(){
    char string[30];    /*in 31 caratteri ci sta tutto l' input tranne eventualmente i valori di autonomia della macchineq*/
    int check;                  
    int* arr;
    char instruction;  /*0 -> aggiungistazione, 1-> demoliscistazione, 2->aggiungiauto,3->rottamaauto,4->pianificapercorso*/
    int i;
    int ok;

    Tnil = (sta*) malloc(sizeof(sta));  //crea Tnil che serve per il RB tree (che non c'è)
    staroot=Tnil;
    inizializzaTNIL();
    instruction = '\0'; /*valore iniziale per garantire che parta*/

    while(instruction != 'n'){    /*per ora così poi rivedo come fare ad uscire*/

        instruction = leggiriga(string);
        switch (instruction)
        {
        case 'a':   /*aggiungi stazione*/            
            arr = (int*) malloc(par2*sizeof(int));  /*crea spazio per le macchine dopo*/
            for(i=0;i<par2;i++)
                ok = scanf("%d",&arr[i]);

            ok++;
            check = aggiungistazione(par1, arr, par2);  
            free (arr);
            if(check == 1)
                printf("aggiunta\n");
            else
                printf("non aggiunta\n");
            break;

        case 't':   /*aggiungi auto*/   
            check = aggiungiauto(par1, par2);
            if(check == 1)
                printf("aggiunta\n");
            else
                printf("non aggiunta\n");
            break;

        case 'd':   /*demolisci auto*/
            check= demoliscistazione(par1);
            if(check == 1)
                printf("demolita\n");
            else
                printf("non demolita\n");
            break;
        case 'o':   /*rottama auto*/
            check = rottamaAuto(par1,par2);
            if(check == 1)
                printf("rottamata\n");
            else
                printf("non rottamata\n");
            break;

        case 'e':   /*pianifica percorso*/
            pianificapercorso(par1, par2);  /*la stampa è gestita dalla funzione*/
            break;
        case 'n':
            break;
        default:
            break;
        }
    }
    /*testa lista dove mettere i valori autonomie*/
    /*comportamento, loop di leggo riga da input, e chiamo le funzioni in base a ciò che ho letto, inoltre inizializza il RBtree delle stazioni all' inizio*/
    return 0;
}