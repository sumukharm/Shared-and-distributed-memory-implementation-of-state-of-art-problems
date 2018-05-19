module Distributed_rw_fapsp
{
	config const message = "Implementing distributed r-way fw apsp in chapel";

	proc convert1_to_2d(idx:int, i:[int], j:[int])
	{
		var j = 0;
		for j in 0..idx do
			j:j+1;
			idx=idx-j;
		i=idx;
	}

	proc convert2_to_1d(i:int, j:int, idx:[int])
	{
		var temp=((j * (j+1))/2) + i;
		return temp;
	}

	proc PAR(X:c_ptr(T), n_local : int, r : int, r_shared : int, my_rank: int, CHPL_COMM_WORLD comm_world )
	{
		for i in 0..r do
			funcA ( X, n_local, n_local, 0, 0 ) if my_rank == convert2_to_1d(i,i))
							    else pass;
		proc Barrier(comm: MPI_Comm);
		proc Group(group_world:MPI_Group) ;
	        proc Comm_group(comm_world: MPI_Comm_group, group_world: [MPI_comm]);
			
		for k in 1..r do
      			proc Barrier(comm_world : MPI_comm);

      			for i in 0..r-k+1 do 
         			var j = i + k;
		
				for l in i+1..j do
					proc Send(X[0]:[c_ptr(x)], n_local : c_ptr(n_local), MPI_INT, convert2_to_1d(i, l):int, 0:int, 	
						comm_world:MPI_Comm_group) if my_rank == convert2_to_1d(l, j);
									   else pass;
					exec Block
					(
						val X_li:c_ptr(int);
						X_li = (int64_t*) chpl_malloc(n_local * n_local * sizeof(int));
						proc Recv(X_li[0]:c_ptr(int), n_local * n_local, CHPL_MPI_INT, convert2_to_1d(l, j):int, 								0:int, comm_world, CHPL_MPI_STATUS_IGNORE);
						funcC(X, X, X_li, n_local, n_local, n_local, n_local, 0, 0, 0, 0);
						CHPL_free(X_li);
					)if (my_rank == convert2_to_1d(i, l)) else pass;

			var n_row = j - i;
         		int64 ranks_row:cptr_t(ranks_row) =  chpl_malloc(n_row * 4);
         		ranks_row[0] = convert2_to_1d(i, j);				
			var idx = 1;
         		for l in i+1..j do
            			ranks_row[idx++] = convert2_to_1d(i, l);
         		
			var group_same_row:CHPL_MPI_GROUP, comm_same_row: CHPL_MPI_Comm;
         		proc MPI_Group_incl(group_world: CHPL_MPI_GROUP, n_row:int, ranks_row:int, group_same_row:[c_ptr[CHPL_MPI_GROUP]);
	 	        proc CHPL_MPI_Comm_create(comm_world:CHPL_MPI_Comm, group_same_row:CHPL_MPI_GROUP, comm_same_row:c_ptr(MPI_Comm));
	
			//pseudo code
			//recursively broadcase to its right, bottom
			//fill out A and B routines for the same.
	}

	proc fill_X(x:c_ptr(T), n_local : int)
	{
		for i in 0..n do
      			var rand_value = n_local % 100 + 1;
      				x[i]= if (rand_value <= 10) then rand_value else (1<<30) 
         		
	}

	proc copy(input : c_ptr(T), output_1 : c_ptr(T), n:int)
	{
		for i in 0..n do
      			output_1[i] = input[i];
	}

	proc main(args: [] string)
	{
		writeln(message);
	
		var n, n_local, r_shared, r;
		int_64t *X, *D;
   		var num_procs,my_rank;
	        CHPL_MPI_Comm comm_col, comm_front_col;
   		CHPL_MPI_Status status;
   		var dashN[..] = "-szn", counter=2;
   		var dashR[..] = "-r_shared";
	   
		exec Block
		(
      			n = str(args[counter+1]);
			exec Block
			(
				r_shared= str_to_int(str(args[counter+1]))
			) if dashR ==args[counter]
			else do
				writeln("The program requires one more argument (proceeded with -r_shared), for the r value of the r-way FW-APSP running on the shared memory part !");
		) if dashN==args[counter]	
			else if dashR==args[counter] do
				r_shared= str_to_int(str(args[counter+1]))
      		

      		CHPL_MPI_Init(argc:[int], args:c_ptr[args]);
   		CHPL_MPI_Comm_size(COMM_WORLD:MPI_Comm_group, num_procs:int);
   		CHPL_MPI_Comm_rank(COMM_WORLD:MPI_Comm_group, my_rank:int);

   		var a = 1, b = 1, c = -2 * num_procs, r = static_cast<int>((-1 * b + sqrt(b * b - 4 * a * c)) / (2 * a)), N = n_local;
		var B = 32;
   		var coord_i = -1;
   		var coord_j = -1;
  		proc convert1_to_2d(my_rank, coord_i, coord_j);
   
   		X = (int64 *) chpl_malloc(n_local * n_local * static_cast<int>(sizeof(int)));
   		X[0:(N*N)] = 0;
   		proc conv_RM_2_ZM_RM( X, D, 0, 0, N, N );
		var start = CHPL_MPI_Wtime();
   		proc PAR(X, n_local, r, r_shared, my_rank, MPI_COMM_WORLD);
   		var end = MPI_Wtime();

   		proc conv_ZM_2_RM_RM( X, D, 0, 0, N, N );
      		writeln("Runtime: ",end-start)  if my_rank==0;
		CHPL_free(X); CHPL_free(D);
	        CHPL_MPI_Finalize();

 		return 0;
  	}

	proc funcA(x:int, xilen:int, xjlen:int, xi:int, xj:int)
	{
		
	}

	proc funcB(x:c_ptr(x), u:c_ptr(u), v:c_ptr(v), xilen:int, xjlen:int, ujlen:int,vilen:int, xi:int, xj:int, uj:int, vi:int)
	{
	
	}

	proc funcC(x:c_ptr(x), u:c_ptr(u), v:c_ptr(v), xilen:int, xjlen:int, ujlen:int, vilen:int, xi:int, xj:int, uj:int, vi:int)
	{

	}
}
