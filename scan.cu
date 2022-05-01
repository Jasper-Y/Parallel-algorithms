#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include <thrust/device_free.h>
#include <thrust/device_malloc.h>
#include <thrust/device_ptr.h>
#include <thrust/scan.h>

void cudaScanThrust(int *host_data, int len)
{
    thrust::device_ptr<int> device_data = thrust::device_malloc<int>(len);

    cudaMemcpy(device_data.get(), host_data, len * sizeof(int),
               cudaMemcpyHostToDevice);

    thrust::inclusive_scan(device_data, device_data + len, device_data);

    cudaDeviceSynchronize();

    cudaMemcpy(host_data, device_data.get(), len * sizeof(int),
               cudaMemcpyDeviceToHost);
    thrust::device_free(device_data);
}


