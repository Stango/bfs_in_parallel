/************************************************************************
filename: gpu/cuda.cu
author: onesuper
email: onesuperclark@gmail.com

allocate and free memory on device
for common use
*************************************************************************/

#include <cuda.h>


void device_alloc_and_copy(void) {

	 cudaMalloc((void**) &d_node_list, sizeof(Node) * num_of_nodes);
	 cudaMemcpy(d_nodes_list, node_list, sizeof(Node) * num_of_nodes,
				cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_edge_list, sizeof(Edge) * num_of_edges);
	 cudaMemcopy(d_edge_list, edge_list. sizeof(Edge) * num_of_edges,
				 cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_color, sizeof(int) * num_of_nodes);
	 cudaMemcopy(d_color, color, sizeof(int) * num_of_nodes,
				 cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_cost, sizeof(int) * num_of_nodes);
	 cudaMemcopy(d_cost, cost, sizeof(int) * num_of_nodes,
				 cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_counter, sizeof(int) * MAX_LEVEL);
	 cudaMemcopy(d_counter, counter, sizeof(int) * MAX_LEVEL,
				 cudaMemcpyHostToDevice);

	 return; 
}

void device_free(void) {
	 cudaFree(d_node_list);
	 cudaFree(d_edge_list);
	 cudaFree(d_cost);
	 cudaFree(d_color);
	 cudaFree(d_counter);
	 return;
}