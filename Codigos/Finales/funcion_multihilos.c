omp_set_num_threads(N_THREADS);
#pragma omp parallel shared(Consultas, DB, N_QUERIES, N_DB, N_THREADS, acum, DIM)
    {
        float real_time;
        struct timeval t1, t2;
        int i, j;
        Elem *heap, e_temp;
        double d;
        int n_elem = 0;
        int tid = omp_get_thread_num(); //ID del thread
        int procs = omp_get_num_threads(); //Nro. total de threads
        double suma = 0;

        suma = 0;
        heap = (Elem *) malloc(sizeof (Elem) * TOPK);

#pragma omp barrier

#pragma omp master
        {
            gettimeofday(&t1, 0);
        }

        //Cada hilo accede a un subconjunto de las consultas. Cada hio accede de manera circular al arreglo de consultas.
        for (i = tid; i < N_QUERIES; i += procs) {
            
            n_elem = 0;
            for (j = 0; j < N_DB; j++) {
               
                d = distancia(Consultas[i], DB[j]);
                //Si la distancia del objeto a la consulta es menor que la raíz del heap, entonces se inserta en el heap. La raíz siempre mantiene la mayor de las distancias
                
                if(n_elem<TOPK){
                    e_temp.dist = d;
                    e_temp.ind = j;
                    inserta2(heap, &e_temp, &n_elem);
                }
                if (n_elem==TOPK){
                if (d < topH(heap, &n_elem)) {
                    e_temp.dist = d;
                    e_temp.ind = j;
                    //Si el heap no está lleno, se inserta el elemento
                    if (n_elem < TOPK)
                        inserta2(heap, &e_temp, &n_elem);
                        //Si el heap está lleno, se inserta el elemento nuevo y se saca el que era antes de mayor de distancia. popush2() hace las operaciones de sacar el elemento mayor e insertar el nuevo.
                    else
                        popush2(heap, &n_elem, &e_temp);
                }}
            }
            
            //En este punto del código se tienen los K elemntos más cercanos a la consulta en 'heap'. Se pueden extraer con extraer2()
            for (j = 0; j < TOPK ; j++) {
                extrae2(heap, &n_elem, &e_temp);
                answer[i*TOPK+j].ind = e_temp.ind;
                answer[i*TOPK+j].dist = e_temp.dist;
            }
            //Realizamos una operación con los resultados para que el compilador no evite hacer instrucciones que considere que el usuario no utiliza. Simplemente cada hilo suma las distancias de los elementos mas cercanos a la consulta 

        }

#pragma omp barrier

#pragma omp master
        {
            gettimeofday(&t2, 0);
            real_time = (t2.tv_sec - t1.tv_sec) + (float) (t2.tv_usec - t1.tv_usec) / 1000000;

            Salida_Multihilo = fopen("Salida_Multihilo.txt", "w");
            for (i = 0; i < N_QUERIES; ++i){
              fprintf(Salida_Multihilo, "Consulta id:: %d\n",i);
                for (j = 0; j < TOPK; ++j){
                    fprintf(Salida_Multihilo,"ind = %d :: dist = %f\n",answer[(i*TOPK)+j].ind,answer[(i*TOPK)+j].dist);
                }
                    fprintf(Salida_Multihilo, "---------------------------------\n");
            }
            fclose(Salida_Multihilo);

            printf("\n\nK = %d", TOPK);
            printf("\nReal Time = %f segundos.\n", real_time);
            fflush(stdout);
        }
        free(heap);

    }//end pragma omp parallel