# include <iostream>
# include <ctime>

using namespace std;

void metodoInsercion_A(int [], int);
void metodoInsercion_D(int [], int);
void swap(int *, int *);
void metodoSeleccion_A(int [], int);
void metodoSeleccion_D(int [], int);
void metodoBurbuja_A(int [], int);
void metodoBurbuja_D(int [], int);
void merge(int [], int, int, int);
void metodoMergeSort_A(int [], int, int);
void mergeD(int [], int [], int, int);
void metodoMergeSort_D(int [], int[], int, int);
void metodoShellSort_A(int [], int);
void metodoShellSort_D(int [], int);
int particion(int [], int, int );
int particionD(int [], int, int );
void metodoQuickSort_A(int [], int, int );
void metodoQuickSort_D(int [], int, int );
void heapAux_A(int [], int, int );
void metodoHeapSort_A(int [], int);
void heapAux_D(int [], int, int );
void metodoHeapSort_D(int [], int);
void mostrarVector(int [], int );
void resultados(double **);
void igualarVectores(int [], int [], int );

int main(void) {
    srand(time(NULL));
    /*Se usan dos vectores auxiliares que serán ordenado (ascendente y descendente) 
    y luego regresará a los valores originales para continuar siendo evaluado en todos los métodos*/
    double t0, t1;
    //int vectorDes_A[15]={1,2,3,6,8,3,8,5,9,12,14,13,17,7,4};
    //int vecDes_D[15]={1,35,2,9,4,2,7,8,3,7,32,64,2,45,7};
    int vectorDes_A[10000];
    int vecDes_D[10000];
    for(int i=0;i<10000;i++){
    	vectorDes_A[i]=rand()%20000+1;
    	vecDes_D[i]=rand()%20000+1;
	}
    int vecAuxOrd_A[10000], vecAuxOrd_D[10000], vecAuxM[10000];
    int tam=10000;
    double mat_De_Tiempos[7][3];
    for(int i=0; i<7; i++) {
        for(int j=0; j<3; j++)
            mat_De_Tiempos[i][j]=0;
    }
    cout<<"Vectores originales:"<<endl;
    mostrarVector(vectorDes_A, tam);
    cout<<"Para ordenaci"<<char(162)<<"n ascendente"<<endl;
    mostrarVector(vecDes_D, tam);
    cout<<"Para ordenaci"<<char(162)<<"n descendente\n"<<endl;
    //Evaluación de Inserción ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoInsercion_A(vecAuxOrd_A, tam);
    t1=clock();
    mat_De_Tiempos[0][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Inserción descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoInsercion_D(vecAuxOrd_D, tam);
    t1=clock();
    mat_De_Tiempos[0][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[0][2]=(mat_De_Tiempos[0][0]+mat_De_Tiempos[0][1])/2;//Guarda el tiempo promedio
    //Evaluación de Selección ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoSeleccion_A(vecAuxOrd_A, tam);
    t1=clock();
    mat_De_Tiempos[1][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Selección descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoSeleccion_D(vecAuxOrd_D, tam);
    t1=clock();
    mat_De_Tiempos[1][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[1][2]=(mat_De_Tiempos[1][0]+mat_De_Tiempos[1][1])/2;//Guarda el tiempo promedio
    //Evaluación de Burbuja ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoBurbuja_A(vecAuxOrd_A, tam);
    t1=clock();
    mat_De_Tiempos[2][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Burbuja descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoBurbuja_D(vecAuxOrd_D, tam);
    t1=clock();
    mat_De_Tiempos[2][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[2][2]=(mat_De_Tiempos[2][0]+mat_De_Tiempos[2][1])/2;//Guarda el tiempo promedio
    //Evaluación de Mergesort ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoMergeSort_A(vecAuxOrd_A, 0, tam-1);
    t1=clock();
    mat_De_Tiempos[3][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Mergesort descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoMergeSort_D(vecAuxOrd_D,vecAuxM, 0, tam-1);
    t1=clock();
    mat_De_Tiempos[3][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[3][2]=(mat_De_Tiempos[3][0]+mat_De_Tiempos[3][1])/2;//Guarda el tiempo promedio
    //Evaluación de Shellsort ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoShellSort_A(vecAuxOrd_A, tam);
    t1=clock();
    mat_De_Tiempos[4][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Shellsort descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoShellSort_D(vecAuxOrd_D, tam);
    t1=clock();
    mat_De_Tiempos[4][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[4][2]=(mat_De_Tiempos[4][0]+mat_De_Tiempos[4][1])/2;//Guarda el tiempo promedio
    //Evaluación de Quicksort ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoQuickSort_A(vecAuxOrd_A, 0, tam-1);
    t1=clock();
    mat_De_Tiempos[5][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Quicksort descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoQuickSort_D(vecAuxOrd_D, 0, tam-1);
    t1=clock();
    mat_De_Tiempos[5][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[5][2]=(mat_De_Tiempos[5][0]+mat_De_Tiempos[5][1])/2;//Guarda el tiempo promedio
    //Evaluación de Heapsort ascendente
    igualarVectores(vectorDes_A, vecAuxOrd_A, tam);
    t0=t1=0; 
    t0=clock();
    metodoHeapSort_A(vecAuxOrd_A, tam);
    t1=clock();
    mat_De_Tiempos[6][0]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de mejor caso
    //Evaluación de Heapsort descendente
    igualarVectores(vecDes_D, vecAuxOrd_D, tam);
    t0=t1=0; 
    t0=clock();
    metodoHeapSort_D(vecAuxOrd_D, tam);
    t1=clock();
    mat_De_Tiempos[6][1]=(double(t1-t0)/CLOCKS_PER_SEC);//Guarda el tiempo de peor caso
    mat_De_Tiempos[6][2]=(mat_De_Tiempos[6][0]+mat_De_Tiempos[6][1])/2;//Guarda el tiempo promedio
    cout<<"Vectores ordenados:"<<endl;
    mostrarVector(vecAuxOrd_A, tam); cout<<" Forma ascendente"<<endl;
    mostrarVector(vecAuxOrd_D, tam); cout<<" Forma descendente\n"<<endl;
    cout<<" M"<<char(130)<<"todo\t    Mejor caso   Peor Caso  Promedio"<<endl;
    cout<<"Inserci"<<char(162)<<"n      "<<mat_De_Tiempos[0][0]<<"      \t  "<<mat_De_Tiempos[0][1]<<"      \t"<<mat_De_Tiempos[0][2]<<endl;
    cout<<"Selecci"<<char(162)<<"n      "<<mat_De_Tiempos[1][0]<<"      \t  "<<mat_De_Tiempos[1][1]<<"      \t"<<mat_De_Tiempos[1][2]<<endl;
    cout<<"Burbuja        "<<mat_De_Tiempos[2][0]<<"      \t  "<<mat_De_Tiempos[2][1]<<"      \t"<<mat_De_Tiempos[2][2]<<endl;
    cout<<"Mergesort      "<<mat_De_Tiempos[3][0]<<"      \t  "<<mat_De_Tiempos[3][1]<<"      \t"<<mat_De_Tiempos[3][2]<<endl;
    cout<<"Shellsort      "<<mat_De_Tiempos[4][0]<<"      \t  "<<mat_De_Tiempos[4][1]<<"      \t"<<mat_De_Tiempos[4][2]<<endl;
    cout<<"Quicksort      "<<mat_De_Tiempos[5][0]<<"      \t  "<<mat_De_Tiempos[5][1]<<"      \t"<<mat_De_Tiempos[5][2]<<endl;
    cout<<"Heapsort       "<<mat_De_Tiempos[6][0]<<"      \t  "<<mat_De_Tiempos[6][1]<<"      \t"<<mat_De_Tiempos[6][2]<<endl;
    return 0;
}

void metodoInsercion_A(int vec[], int tam) {
    int indi, j;
    for(int i=1; i<tam; i++) {
        indi=vec[i];
        j=i-1;
        while(j>=0 && vec[j]>indi) {
            vec[j+1]=vec[j];
            j=j-1;
        }
        vec[j+1]=indi;
    }
}
void metodoInsercion_D(int vec[], int tam) {
    int indi, j;
    for(int i=1; i<tam; i++) {
        indi=vec[i];
        j=i-1;
        while(j>=0 && vec[j]<indi) {
            vec[j+1]=vec[j];
            j=j-1;
        }
        vec[j+1]=indi;
    }
}
void swap(int *x1, int *x2) {
    int temp=*x1;
    *x1=*x2;
    *x2=temp;
}
void metodoSeleccion_A(int vec[], int tam) {
    int temp=0, min;
    for(int i=0; i<tam-1; i++) {
        min=i;
        for(int j=i+1; j<tam; j++)
        if(vec[j]<vec[min])
            min=j;
        swap(&vec[min], &vec[i]);
    }
}
void metodoSeleccion_D(int vec[], int tam) {
    int temp=0, min;
    for(int i=0; i<tam-1; i++) {
        min=i;
        for(int j=i+1; j<tam; j++)
        if(vec[j]>vec[min])
            min=j;
        swap(&vec[min], &vec[i]);
    }
}
void metodoBurbuja_A(int vec[], int tam) {
    bool cambio;
    for(int i=0; i<tam-1; i++) {
        cambio=false;
        for(int j=0; j<tam-i-1; j++) {
            if(vec[j]>vec[j+1]) {
                swap(&vec[j], &vec[j+1]);
                cambio=true;
            }
        }
        if(!cambio)
            break;
    }
}
void metodoBurbuja_D(int vec[], int tam) {
    bool cambio;
    for(int i=0; i<tam-1; i++) {
        cambio=false;
        for(int j=0; j<tam-i-1; j++) {
            if(vec[j]<vec[j+1]) {
                swap(&vec[j], &vec[j+1]);
                cambio=true;
            }
        }
        if(!cambio)
            break;
    }
}
void merge(int vec[], int iz, int en, int der) {
    int n1=en-iz+1;
    int n2=der-en;
    int *L=new int[n1], *R=new int[n2];
    for(int i=0; i<n1; i++)
        L[i]=vec[iz+i];
    for(int i=0; i<n2; i++)
        R[i]=vec[en+1+i];
    int i=0, j=0, k=iz;
    while (i<n1 && j<n2) {
        if(L[i]<=R[j]) {
            vec[k]=L[i];
            i++;
        } else {
            vec[k]=R[j];
            j++;
        }
        k++;
    }
    while(i<n1) {
        vec[k]=L[i];
        i++; k++;
    }
    while(j<n2) {
        vec[k]=R[j];
        j++; k++;
    }
}
void metodoMergeSort_A(int vec[],int ini, int tam) {
    if(ini>=tam)
        return;
    int m=(ini+tam-1)/2;
    metodoMergeSort_A(vec, ini, m);
    metodoMergeSort_A(vec, m+1, tam);
    merge(vec, ini, m, tam);
}
void mergeD(int vec[], int aux[], int izq, int der) {
    int indM=(izq+der)/2;
    int indIz=izq;
    int indDer=indM+1;
    int indAux=izq;
    while(indIz<=indM && indDer<=der) {
        if(vec[indIz]>=vec[indDer])
            aux[indAux]=vec[indIz++];
        else
            aux[indAux]=vec[indDer++];
        indAux++;
    }
    while(indIz<=indM) {
        aux[indAux]=vec[indIz++];
        indAux++;
    }
    while (indDer<=der) {
        aux[indAux]=vec[indDer++];
        indAux++;
    }
}
void metodoMergeSort_D(int vec[], int aux[], int ini, int tam) {
    if(ini>=tam)
        return;
    int iniM=(ini+tam)/2;
    metodoMergeSort_D(vec, aux, ini, iniM);
    metodoMergeSort_D(vec, aux, iniM+1, tam);
    mergeD(vec, aux, ini, tam);
    for(int i=ini; i<=tam; i++)
        vec[i]=aux[i];
}
void metodoShellSort_A(int vec[], int tam) {
    for(int parti=tam/2; parti>0; parti/=2) {
        for(int i=parti; i<tam; i++) {
            int temp=vec[i];
            int j;
            for(j=i; j>=parti && vec[j-parti]>temp; j-=parti)
                vec[j]=vec[j-parti];
            vec[j]=temp;
        }
    }
}
void metodoShellSort_D(int vec[], int tam) {
    for(int parti=tam/2; parti>0; parti/=2) {
        for(int i=parti; i<tam; i++) {
            int temp=vec[i];
            int j;
            for(j=i; j>=parti && vec[j-parti]<temp; j-=parti)
                vec[j]=vec[j-parti];
            vec[j]=temp;
        }
    }
}
int particion(int vec[], int men, int may) {
    int apoyo=vec[may];
    int i=men-1;
    for(int j=men; j<=may-1; j++) {
        if(vec[j]<apoyo) {
            i++;
            swap(&vec[i], &vec[j]);
        }
    }
    swap(&vec[i+1], &vec[may]);
    return (i+1);
}
void metodoQuickSort_A(int vec[], int men, int may) {
    if(men<may) {
        int indi=particion(vec, men, may);
        metodoQuickSort_A(vec, men, indi-1);
        metodoQuickSort_A(vec, indi+1, may);
    }
}
int particionD(int vec[], int men, int may){
    int apoyo=vec[may];
    int i=men-1;
    for(int j=men; j<=may-1; j++) {
        if(vec[j]>apoyo) {
            i++;
            swap(&vec[i], &vec[j]);
        }
    }
    swap(&vec[i+1], &vec[may]);
    return (i+1);
}
void metodoQuickSort_D(int vec[], int men, int may) {
    if(men<may) {
        int indi=particionD(vec, men, may);
        metodoQuickSort_D(vec, men, indi-1);
        metodoQuickSort_D(vec, indi+1, may);
    }
}
void heapAux_A(int vec[], int n, int i) {
    int may=i;
    int izq=2*i+1;
    int der=2*i+2;
    if(izq<n && vec[izq]>vec[may]) 
        may=izq;
    if(der<n && vec[der]>vec[may])
        may=der;
    if(may!=i) {
        swap(&vec[i], &vec[may]);
        heapAux_A(vec, n, may);
    }
}
void metodoHeapSort_A(int vec[], int tam) {
    for(int i=tam/2-1; i>=0; i--)
        heapAux_A(vec, tam, i);
    for(int i=tam-1; i>0; i--) {
        swap(&vec[0], &vec[i]);
        heapAux_A(vec, i, 0);
    }
}
void heapAux_D(int vec[], int n, int i) {
    int men=i;
    int izq=2*i+1;
    int der=2*i+2;
    if(izq<n && vec[izq]<vec[men]) 
        men=izq;
    if(der<n && vec[der]<vec[men])
        men=der;
    if(men!=i) {
        swap(&vec[i], &vec[men]);
        heapAux_D(vec, n, men);
    }
}
void metodoHeapSort_D(int vec[], int tam) {
    for(int i=tam/2-1; i>=0; i--)
        heapAux_D(vec, tam, i);
    for(int i=tam-1; i>=0; i--) {
        swap(&vec[0], &vec[i]);
        heapAux_D(vec, i, 0);
    }
}
void mostrarVector(int vec[], int tam) {
    for(int i=0; i<tam; i++) 
        cout<<'['<<vec[i]<<"] ";
}
void igualarVectores(int origen[], int veA_Cam[], int tam) {
    for(int i=0; i<tam; i++) {
        veA_Cam[i]=origen[i];
    }
}
