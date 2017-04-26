   
   #pragma offload target(mic:0) in(dim) in(db_vector:length(num_db*dimaux)) in(queries_vector:length(num_queries*dimaux)) out(answer:length(k*num_queries))
   {
      #pragma omp parallel private(i, j, thread_num) shared(db_vector, num_db, queries_vector, num_queries, dimaux, k, answer, num_threads)
      {
         
         Elem *heap;
         heap = (Elem *)malloc(sizeof(Elem)*k);
         #pragma omp master
         {
            num_threads = omp_get_num_threads();
            printf("run with %d threads\n", num_threads);
         }
         #pragma omp barrier
         thread_num = omp_get_thread_num();
         int n_elem;
         Elem e_temp;
         double d;

         
         for(i=thread_num*dimaux; i<num_queries*dimaux; i+=num_threads*dimaux){
            n_elem = 0;
            for(j=0; j<k; j++){
               e_temp.dist = distancia(&(queries_vector[i]), &(db_vector[j*dimaux]), dimaux);
               e_temp.ind = j;
               inserta2(heap, &e_temp, &n_elem);
            }

            for(j=k; j<num_db; j++){
               d = distancia(&(queries_vector[i]), &(db_vector[j*dimaux]), dimaux);
               if(d < topH(heap, &n_elem))
                  {
                     e_temp.dist = d;
                     e_temp.ind = j;
                     popush2(heap, &n_elem, &e_temp);
                  }
            }

            for(j=0; j<k; j++){
               extrae2(heap, &n_elem, &e_temp);
               printf("%d ind = %d :: dist = %f posición:: %d \n", j, e_temp.ind, e_temp.dist,(i/dimaux)*k+j);
               answer[(i/dimaux)*k+j].ind = e_temp.ind;
               answer[(i/dimaux)*k+j].dist = e_temp.dist;
            }


         }
         free(heap);
      }
   }