/* 
 * File:   main.cpp
 * Author: Cecilia
 *
 * Created on 23 de abril de 2015, 06:26 PM
 */

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <math.h>
#define MAX_MON 5
#define MAX_INF (1<<30)
#define sqr(x) ((x)*(x))
#define MAX_DOUBLE 100000000.0
#define MIN_DOUBLE -100000000.0

using namespace std;


/*
 * 
 */

double calcular_distancia(double * data, double * centroides, int cantAtributos){
    double distance_sq_sum = 0;
    
    for (int i = 0; i < cantAtributos; i++)
      distance_sq_sum += sqr(centroides[i] - data[i]);

    return sqrt(distance_sq_sum);
}

void asignar_items(double ** data, double ** centroides, int cantAtributos, int cantDatos,
        int cantClusters, int* arrClasif){
    //int *arrClasif = new int[cantDatos];
    double dist_calc, min_dist=MAX_INF;
    //para cada punto, calcular su distancia a cada cluster
    for(int i=0;i<cantDatos;i++){
        min_dist=MAX_INF;
        for(int j=0; j<cantClusters;j++){
            dist_calc= calcular_distancia(data[i], centroides[j],cantAtributos);
            if(dist_calc<min_dist) {
                min_dist= dist_calc;
                arrClasif[i]=j;
            }
        }
    }
}

void calcular_media_cluster(double **data, int cantAtributos, int cantDatos,
        double * media, int *arrClasif, int numCluster){
    int numClasificados=0;
    //inicializamos arreglo de medias
    for(int k=0;k<cantAtributos;k++)
        media[k]=0;
    //calculamos la media para cada dimension (atributo)
    for(int i= 0; i< cantDatos; i++){
        if(arrClasif[i]== numCluster){
            for(int j=0;j<cantAtributos;j++){
                media[j] += data[i][j];
            }
            numClasificados++;
        }
    }
    if(numClasificados!=0)
        for (int i=0;i<cantAtributos;i++)
            media[i] /= (numClasificados*1.0);
}

double* mover_centroides(double **centroides, int cantClusters, int cantAtributos, 
        int *arrClasif, double **data, int cantDatos){
    double * media = new double[cantAtributos];
    double dist_movida;
    double *distancias= new double [cantClusters];
    
    for(int i=0;i<cantClusters;i++){
        //para cada centroide, calcular la media, cuánto se mueve y actualizar
        //su posicion
        calcular_media_cluster(data, cantAtributos, cantDatos, media, arrClasif, i);
        dist_movida = calcular_distancia(media, centroides[i], cantAtributos);
        for(int j=0;j<cantAtributos;j++)
            centroides[i][j]=media[j];
        distancias[i]=dist_movida;
    }
    return distancias; //????
}

void definir_centroides(double ** centroides, int cantClusters, int cantAtributos,
                        double *arrMin,double *arrMax){
    for(int i=0;i<cantClusters;i++){
        for(int j=0;j<cantAtributos;j++){
            int pivote=round((arrMax[j]-arrMin[j])/cantClusters);
            centroides[i][j]= arrMin[j]+ pivote*i + (rand() % pivote);
        }
    }
    for(int i=0;i<cantClusters;i++){
        for(int j=0;j<cantAtributos;j++){
            printf("%lf ",centroides[i][j]);
        }
        printf("\n");
    }
}

void clusterizacion(int cantAtributos, int cantClusters, double ** data, int cantDatos,
                    double *arrMin,double *arrMax){
    double ** centroides = new double *[cantClusters];
    for(int i=0;i<cantClusters;i++){
        double *temp = new double[cantAtributos];
        centroides[i]=temp;
    }
    int * arrClasif= new int [cantDatos];
    //definir centroides iniciales;
    definir_centroides(centroides, cantClusters, cantAtributos,arrMin,arrMax); //falta hacer bien esta funcion
    int convergencia = 0;
    double conv_min = MAX_MON;
    while(!convergencia){
        asignar_items(data, centroides, cantAtributos, cantDatos, cantClusters, 
                arrClasif);
        double *dist_movida = mover_centroides(centroides, cantClusters, cantAtributos, arrClasif, data,
                cantDatos);
        for(int i=0;i<cantClusters;i++)
            if(dist_movida[i]<conv_min)
                convergencia++;
        if(convergencia==cantClusters)
            convergencia =1;
        else convergencia =0;
    }
    for(int i=0;i<cantClusters;i++){
        for(int j=0;j<cantAtributos;j++){
            printf("%lf ",centroides[i][j]);
        }
        printf("\n");
    }    
    ofstream output;
    output.open("k_output.csv",ios::out);
    for(int i=0;i<cantDatos;i++){
        for(int j =0;j<cantAtributos;j++)
            output<<data[i][j]<<",";
        output<<arrClasif[i]<<endl;
    }
    output.close();
    
}

int main(int argc, char** argv) { 
    //aca debe leer el archivo, sacar la cantidad de datos y ponerlo todo
    //en una matriz doble donde mat[cantDatos][cantAtributos]
       
    char * filename = new char [100];
    int cantAtributos, clusters;
    printf("escriba el nombre de archivo\n");
    cin>>filename;
    printf("escriba la cantidad de atributos\n");
    cin>>cantAtributos;
    printf("escriba la cantidad de clusters\n");

    cin>>clusters;
    char a;

    
    //en la lecutra de datos falta hacer restricciones como 
    //si no encuentra uno de los atributos no debe tomar en cuenta el registro
    
    ifstream input;
    
    input.open(filename, ios_base::in);
    int cantDatos=0;
    char * numero = new char [50];
    double * dataTemp= new double[cantAtributos];
    while(!input.eof() && input){
        for(int j = 0; j<cantAtributos;j++){
            //input>>dataTemp[j];
            //if(j!=cantAtributos-1)input>>a;
            input.getline(numero, 100,',');
            dataTemp[j]=atoi(numero)*1.0;
            cout<<dataTemp[j]<<endl;
        }
        if(input.eof())
                break;
        cantDatos++;
    }
    input.close();

    double **data = new double* [cantDatos];
    for(int i=0;i<cantDatos;i++){
        double *temp = new double[cantAtributos];
        data[i]=temp;
    }
    ifstream inputD;
    inputD.open(filename, ios::in);
    inputD.seekg(ios_base::beg);
    double *arrMin= new double[cantAtributos];
    double *arrMax= new double[cantAtributos];
    for(int n=0;n<cantAtributos;n++){
        arrMin[n]=MAX_DOUBLE;
        arrMax[n]=MIN_DOUBLE;
    }
    for(int i=0;i<cantDatos;i++){
        for(int j=0;j<cantAtributos;j++){
                inputD.getline(numero, 100,',');
                double temp=atoi(numero)*1.0;
                if(temp < arrMin [j]) arrMin[j]=temp;
                if(temp > arrMax [j]) arrMax[j]=temp;
                data[i][j]=temp;
                cout<<data[i][j]<<endl;
        }
    }
    clusterizacion(cantAtributos, clusters, data, cantDatos,arrMin,arrMax);
    
    cin>>a;
    cout<<a;

    return 0;
}

