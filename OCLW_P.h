/********************************************************************************************************************************
OpenCL Wrapper By Punal Manalan
Use this code wherever and whenever you want to!

Information About This Wrapper:
1. LightWeight, Easy to use **Header-Only** Wrapper for OpenCL.
2. 'OCLW_P::OpenCLWrapper' This Single line of code does Everything(Including Adding Functions) Automaticaly.
3. Written Using only "CL/cl.h" and has backward compatblity upto OpenCL 1.2!
3. No need to maually Add kernel function or figure out argument types.
4. The Entire OpenCL Program File is Read and automaticaly Assignes Kernel Functions Ready for use!
5. Information about each kernel function and it's argument types can be Accesed with **getter**functions().
6. Compiles and Runs on Windows, Linux and Mac.
7. This Wrapper specializes in Heavy Computation on GPU.
8. From Constrction to Destruction everything is Done automaticaly!
9. No need for manual destruction and releasing cl resources!
10. No need to write Tedious amount of code to counter memory leaks!
11. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
*********************************************************************************************************************************/

#ifndef OPENCL_WRAPPER_BY_PUNAL
#define OPENCL_WRAPPER_BY_PUNAL
#pragma once

#define __CL_ENABLE_EXCEPTIONS
#define CL_API_SUFFIX__VERSION_1_2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
//#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>// OpenCl

#include "Essenbp.h"//Helper Functions and Struct By Punal

namespace OCLW_P//OpenCL Wrapper By Punal Manalan
{
	enum class cl_Memory_Type//NOTE: This is a Enum Based On CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE for buffer creation in the cl_device
	{
		Uninitialized_cl_Memory = 0,//NULL,	
		CL_READ_ONLY = 1,//CL_MEM_READ_ONLY,
		CL_WRITE_ONLY = 2,//CL_MEM_WRITE_ONLY,
		CL_READ_AND_WRITE = 3,//CL_MEM_READ_WRITE,
		CL_LOCALENUM = 4,//5,// Not To be confused with CL_LOCAL
		CL_PRIVATE = 5//3
	};

	// NOTE: Call Constructor Before Using
	struct cl_PlatformVendorStruct
	{
	private:
		bool IsPlatformChosen = false;
		cl_uint ChosenPlatform = 0;
		cl_uint TotalNumberOfPlatformVendors = 0;
		std::string** AllAvailablePlatformVendorNames = nullptr;

	public:
		bool IsConstructionSuccesful = false;
		cl_PlatformVendorStruct(bool& Issuccessful)
		{
			std::cout << "\n Constructing cl_PlatformVendorStruct!";

			IsPlatformChosen = false;
			ChosenPlatform = 0;
			TotalNumberOfPlatformVendors = 0;
			AllAvailablePlatformVendorNames = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			cl_int ClErrorResult = clGetPlatformIDs(0, NULL, &TotalNumberOfPlatformVendors);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting platforms In cl_PlatformVendorStruct!\n";
			}
			else
			{
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&AllAvailablePlatformVendorNames, TotalNumberOfPlatformVendors, sizeof(std::string*), Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error Allocating :" << TotalNumberOfPlatformVendors * sizeof(std::string*) << " Byes Of Memory for AllAvailablePlatformVendorNames cl_PlatformVendorStruct!\n";
				}
			}

			if (Issuccessful)// For the safe of readability
			{
				cl_platform_id PlatformID = nullptr;
				for (cl_uint i = 0; i < TotalNumberOfPlatformVendors; ++i)
				{
					Issuccessful = false;
					ClErrorResult = clGetPlatformIDs(i + 1, &PlatformID, NULL);// for the first platform //Single Line code
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n ClError Code " << ClErrorResult << " : Platform Not set In cl_PlatformVendorStruct!\n";						
						free(AllAvailablePlatformVendorNames);
						AllAvailablePlatformVendorNames = nullptr;
						break;
					}
					else
					{
						char* ReturnValue = NULL;
						size_t SizeOfReturnValue;

						ClErrorResult = clGetPlatformInfo(PlatformID, CL_PLATFORM_VENDOR, NULL, ReturnValue, &SizeOfReturnValue); // Get the size of the ReturnValue
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Getting Size of CL_PLATFORM_VENDOR in clGetPlatformInfo In cl_PlatformVendorStruct!\n";
							free(AllAvailablePlatformVendorNames);
							AllAvailablePlatformVendorNames = nullptr;
							break;
						}

						ReturnValue = (char*)malloc(SizeOfReturnValue * sizeof(char));
						if (ReturnValue == nullptr)
						{
							std::cout << "\n Error Allocating :" << sizeof(char) * SizeOfReturnValue << " Byes Of Memory for ReturnValue cl_PlatformVendorStruct!\n";
							free(AllAvailablePlatformVendorNames);
							AllAvailablePlatformVendorNames = nullptr;
							break;
						}

						ClErrorResult = clGetPlatformInfo(PlatformID, CL_PLATFORM_VENDOR, SizeOfReturnValue, ReturnValue, NULL); // Get the Return Value
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Getting Value of CL_PLATFORM_VENDOR in clGetPlatformInfo In cl_PlatformVendorStruct!\n";
							free(ReturnValue);
							free(AllAvailablePlatformVendorNames);
							AllAvailablePlatformVendorNames = nullptr;
							break;
						}
						else
						{
							AllAvailablePlatformVendorNames[i] = new std::string(ReturnValue);
							Issuccessful = AllAvailablePlatformVendorNames[i] != nullptr;
							free(ReturnValue);
							if (!Issuccessful)
							{
								std::cout << "\n Error Allocating :" << sizeof(std::string*) << " Byes Of Memory for AllAvailablePlatformVendorNames[" << i << "] cl_PlatformVendorStruct!\n";
								for (int j = 0; j < i; ++j)
								{
									delete AllAvailablePlatformVendorNames[j];
								}
								free(AllAvailablePlatformVendorNames);
								AllAvailablePlatformVendorNames = nullptr;
								Issuccessful = false;
								break;
							}
						}
					}
				}
				
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_PlatformVendorStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		//NOTE: Mininum is 1 maximum is TotalNumberOfPlatformVendors
		void GetPlatformVendorName(int ArgPlatformNumber, std::string& PlatformVendorName, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetTotalNumberOfPlatformVendors Without Constructing the struct In: cl_PlatformVendorStruct!\n";
			}
			else
			{
				if (ArgPlatformNumber > TotalNumberOfPlatformVendors)
				{
					std::cout << "\n Error ArgPlatformNumber is greater than TotalNumberOfPlatformVendors in GetPlatformVendorName In cl_PlatformVendorStruct!\n";
				}
				else
				{
					PlatformVendorName = *(AllAvailablePlatformVendorNames[ArgPlatformNumber]);
					Issuccessful = true;					
				}
				
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetPlatformVendorName() Failed in cl_PlatformVendorStruct!\n";
			}
		}
		void GetTotalNumberOfPlatformVendors(int& ArgTotalNumberOfPlatformVendors, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetTotalNumberOfPlatformVendors Without Constructing the struct In: cl_PlatformVendorStruct!\n";
			}
			else
			{
				ArgTotalNumberOfPlatformVendors = TotalNumberOfPlatformVendors;
				Issuccessful = true;
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetTotalNumberOfPlatformVendors() Failed in cl_PlatformVendorStruct!\n";
			}
		}
		void GetChosenPlatform(cl_uint& ArgChosenPlatform, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetChosenPlatform Without Constructing the struct In: cl_PlatformVendorStruct!\n";
			}
			else
			{
				if (!IsPlatformChosen)
				{
					std::cout << "\n Error Platform Is Not Chosen In GetChosenPlatform In: cl_PlatformVendorStruct!\n";
				}
				else
				{
					ArgChosenPlatform = ChosenPlatform;
					Issuccessful = true;
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetChosenPlatform() Failed in cl_PlatformVendorStruct!\n";
			}
		}

		//NOTE: ChosenPlatform is always greater than 1 and less than Number Of Platform + 1
		void SetChosenPlatform(cl_uint ArgChosenPlatform, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetChosenPlatform Without Constructing the struct In: cl_PlatformVendorStruct!\n";
			}
			else
			{
				if (ArgChosenPlatform > TotalNumberOfPlatformVendors)
				{ 
					std::cout << "\n Error ArgChosenPlatform is greater than TotalNumberOfPlatformVendors in GetPlatformVendorName In cl_PlatformVendorStruct!\n";
				}
				else
				{
					ChosenPlatform = ArgChosenPlatform;
					IsPlatformChosen = true;
					Issuccessful = true;
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetChosenPlatform() Failed in cl_PlatformVendorStruct!\n";
			}
		}

		void PrintAllAvailablePlatformVendorNames(bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PrintAllAvailablePlatformVendorNames Without Constructing the struct In: cl_PlatformVendorStruct!\n";
			}
			else
			{
				std::string PlatformVendorName;
				for (int i = 0; i < TotalNumberOfPlatformVendors; ++i)
				{
					GetPlatformVendorName(i, PlatformVendorName, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error: Function Unsuccesful In: cl_PlatformVendorStruct!\n";
						break;
					}
					std::cout << "\n Platform Vendor Nunber: '" << i + 1 << "', Platform Vendor Name: '" << PlatformVendorName << "'\n";
				}
			}
			
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error PrintAllAvailablePlatformVendorNames() Failed in cl_PlatformVendorStruct!\n";
			}
		}

		~cl_PlatformVendorStruct()
		{
			std::cout << "\n Destructing cl_PlatformVendorStruct!";
			if (IsConstructionSuccesful)
			{
				for (cl_uint i = 0; i < TotalNumberOfPlatformVendors; ++i)
				{
					delete AllAvailablePlatformVendorNames[i];
				}
				free(AllAvailablePlatformVendorNames);
				AllAvailablePlatformVendorNames = nullptr;
				IsPlatformChosen = false;
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_PerDeviceValuesStruct
	{
		const cl_device_id SelectedDevice;
		cl_command_queue DeviceClCommandQueue = NULL;
		size_t MaxComputeUnitPerGPU = 0;
		size_t MaxWorkItemPerGroup = 0;
		size_t MaxGlobalMemoryOfDevice = 0;
		size_t MaxPrivateMemoryBytesPerWorkGroup = 0;
		size_t MaxLocalMemoryBytesPerWorkGroup = 0;
		bool IsConstructionSuccesful = false;// Constructor argument cl_int& ClErrorResult returns our succes value so 'IsConstructionSuccesful' Is only needed when exiting program

		cl_PerDeviceValuesStruct(cl_device_id ArgSelectedDevice, cl_context* TheClContext, cl_int& ClErrorResult, bool& Issuccessful) : SelectedDevice(ArgSelectedDevice)
		{
			std::cout << "\n Constructing cl_PerDeviceValuesStruct!";

			DeviceClCommandQueue = NULL;
			MaxComputeUnitPerGPU = 0;
			MaxWorkItemPerGroup = 0;
			MaxGlobalMemoryOfDevice = 0;
			MaxPrivateMemoryBytesPerWorkGroup = 0;
			MaxLocalMemoryBytesPerWorkGroup = 0;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			DeviceClCommandQueue = clCreateCommandQueue(*TheClContext, SelectedDevice, 0, &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : cl_CommandQueue Creation In: ccl_PerDeviceValuesStruct!\n";
			}
			else
			{
				cl_uint Temp1 = 0;
				size_t Temp2 = 0;
				cl_ulong Temp3 = 0;// Calloc Called Below

				ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &Temp1, NULL);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_COMPUTE_UNITS Info Failed!\n";
				}
				else
				{
					MaxComputeUnitPerGPU = (int)Temp1;
					std::cout << "\nMaxComputeUnitPerGPU:\n";
					std::cout << MaxComputeUnitPerGPU << "\n";

					ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!\n";
					}
					else
					{
						MaxWorkItemPerGroup = (int)Temp2;
						std::cout << "\nMaxWorkItemPerGroup:\n";
						std::cout << MaxWorkItemPerGroup << "\n";

						ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!\n";
						}
						else
						{
							MaxGlobalMemoryOfDevice = Temp3;
							std::cout << "\nMaxGlobalMemoryOfDevice:\n";// Total GPU VRAM 
							std::cout << MaxGlobalMemoryOfDevice << "\n";

							ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!\n";
							}
							else
							{
								MaxPrivateMemoryBytesPerWorkGroup = (int)Temp3;
								std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup:\n";// Per Work Item
								std::cout << MaxPrivateMemoryBytesPerWorkGroup << "\n";

								ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!\n";	
								}
								else
								{
									MaxLocalMemoryBytesPerWorkGroup = (int)Temp3;
									std::cout << "\nMaxLocalMemoryBytesPerWorkGroup:\n";// Per Work Group
									std::cout << MaxLocalMemoryBytesPerWorkGroup << "\n\n";
									Issuccessful = true;
								}
							}
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PlatformVendorStruct!";
				DeviceClCommandQueue = NULL;
				MaxComputeUnitPerGPU = 0;
				MaxWorkItemPerGroup = 0;
				MaxGlobalMemoryOfDevice = 0;
				MaxPrivateMemoryBytesPerWorkGroup = 0;
				MaxLocalMemoryBytesPerWorkGroup = 0;
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		~cl_PerDeviceValuesStruct()
		{
			std::cout << "\n Destructing cl_PerDeviceValuesStruct!";
			if (IsConstructionSuccesful)
			{
				cl_int ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				DeviceClCommandQueue = NULL;
				MaxComputeUnitPerGPU = 0;
				MaxWorkItemPerGroup = 0;
				MaxGlobalMemoryOfDevice = 0;
				MaxPrivateMemoryBytesPerWorkGroup = 0;
				MaxLocalMemoryBytesPerWorkGroup = 0;
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_NDRangeStruct//Maximum Dimensions is 3
	{
	private:
		cl_uint Dimensions = 0;		// Work Dimensions
		size_t GlobalWorkSize[3] = {0,0,0};	// Total Number Of Work Items
		size_t LocalWorkSize[3] = { 0,0,0 };	// Work Items per Work Group
		size_t WorkSizeOffset[3] = { 0,0,0 };	// WorkSize Offset

	public:
		bool IsConstructionSuccesful = false;
		cl_NDRangeStruct(bool& Issuccessful)
		{
			std::cout << "\n Constructing cl_NDRangeStruct!";

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if((GlobalWorkSize != nullptr) || (LocalWorkSize != nullptr) || (WorkSizeOffset != nullptr))// In Case someting Unexpected happens
			{
				Dimensions = 0;
				GlobalWorkSize[0] = 0;
				GlobalWorkSize[1] = 0;
				GlobalWorkSize[2] = 0;

				LocalWorkSize[0] = 0;
				LocalWorkSize[1] = 0;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = 0;
				WorkSizeOffset[1] = 0;
				WorkSizeOffset[2] = 0;
				Issuccessful = true;
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_NDRangeStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		// Deprecated NOTE: No Use
		/*
		//NOTE: This sets everything to 0 which means enqueuendrangekernel should be passed with null for both global and local work sizes
		void SetNDRange(cl_uint ArgDimensions)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = ArgDimensions;//By default it is 1
			GlobalWorkSize[0] = 0;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = 0;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;
		}*/

		//NOTE: 1 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRange() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 1;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = 0;
				GlobalWorkSize[2] = 0;

				LocalWorkSize[0] = LocalSize1D;
				LocalWorkSize[1] = 0;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = 0;
				WorkSizeOffset[2] = 0;
			}
		}

		//NOTE: 2 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t LocalSize2D, size_t WorkSizeOffset2D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRange() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 2;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = GlobalSize2D;
				GlobalWorkSize[2] = 0;

				LocalWorkSize[0] = LocalSize1D;
				LocalWorkSize[1] = LocalSize2D;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = WorkSizeOffset2D;
				WorkSizeOffset[2] = 0;
			}
		}

		//NOTE: 3 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t LocalSize2D, size_t WorkSizeOffset2D, size_t GlobalSize3D, size_t LocalSize3D, size_t WorkSizeOffset3D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRange() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 3;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = GlobalSize2D;
				GlobalWorkSize[2] = GlobalSize3D;

				LocalWorkSize[0] = LocalSize1D;
				LocalWorkSize[1] = LocalSize2D;
				LocalWorkSize[2] = LocalSize3D;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = WorkSizeOffset2D;
				WorkSizeOffset[2] = WorkSizeOffset3D;
			}
		}

		//NOTE: 1 Dimension With Local 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRangeWithoutLocal() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 1;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = 0;
				GlobalWorkSize[2] = 0;

				LocalWorkSize[0] = 0;
				LocalWorkSize[1] = 0;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = 0;
				WorkSizeOffset[2] = 0;
			}
		}

		//NOTE: 2 Dimension With Local 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t WorkSizeOffset2D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRangeWithoutLocal() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 2;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = GlobalSize2D;
				GlobalWorkSize[2] = 0;

				LocalWorkSize[0] = 0;
				LocalWorkSize[1] = 0;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = WorkSizeOffset2D;
				WorkSizeOffset[2] = 0;
			}
		}

		//NOTE: 3 Dimension With Local 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t WorkSizeOffset2D, size_t GlobalSize3D, size_t WorkSizeOffset3D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				std::cout << "\n Error SetNDRangeWithoutLocal() Failed in cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				Dimensions = 3;//By default it is 1
				GlobalWorkSize[0] = GlobalSize1D;
				GlobalWorkSize[1] = GlobalSize2D;
				GlobalWorkSize[2] = GlobalSize3D;

				LocalWorkSize[0] = 0;
				LocalWorkSize[1] = 0;
				LocalWorkSize[2] = 0;

				WorkSizeOffset[0] = WorkSizeOffset1D;
				WorkSizeOffset[1] = WorkSizeOffset2D;
				WorkSizeOffset[2] = WorkSizeOffset3D;
			}
		}	

		void GetNDRange(cl_uint* ArgDimensions, size_t** ArgGlobalSize, size_t** ArgLocalSize, size_t** ArgWorkSizeOffset, bool& Issuccessful)
		{
			Issuccessful = true;//Yes this one is set to true				

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
			}
			else
			{
				if (Dimensions == 0)
				{
					std::cout << "\n Error Calling GetNDRange Without SettingNDRange First using SetNDRange() In: cl_NDRangeStruct!\n";
				}
				else
				{	
					if (ArgDimensions == nullptr)
					{
						std::cout << "\n Error ArgDimensions is nullptr in SetNDRange() In: cl_NDRangeStruct!\n";
						Issuccessful = false;
					}
					if (ArgGlobalSize == nullptr)
					{
						std::cout << "\n Error ArgGlobalSize is nullptr in SetNDRange() In: cl_NDRangeStruct!\n";
						Issuccessful = false;
					}
					if (ArgLocalSize == nullptr)
					{
						std::cout << "\n Error ArgLocalSize is nullptr in SetNDRange() In: cl_NDRangeStruct!\n";
						Issuccessful = false;
					}
					if (ArgWorkSizeOffset == nullptr)
					{
						std::cout << "\n Error ArgWorkSizeOffset is nullptr in SetNDRange() In: cl_NDRangeStruct!\n";
						Issuccessful = false;
					}

					if (Issuccessful)
					{
						// Temporarly using this variable for different purpose
						Issuccessful = (LocalWorkSize[0] == 0) || (LocalWorkSize[1] == 0) || (LocalWorkSize[2] == 0);
						*ArgDimensions = Dimensions;
						*ArgGlobalSize = GlobalWorkSize;
						if (Issuccessful)
						{
							*ArgLocalSize = nullptr;
							*ArgWorkSizeOffset = nullptr;
						}
						else
						{
							*ArgLocalSize = LocalWorkSize;
							*ArgWorkSizeOffset = WorkSizeOffset;
						}
					}
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetNDRange() Failed in cl_MultiDevice_NDRangeStruct!\n";
			}
		}

		void CopyNDRange(cl_NDRangeStruct* ArgNDRange, bool& Issuccessful)
		{
			Issuccessful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
			}
			else
			{
				cl_uint ArgDimensions = 0;//By default it is 1
				size_t* ArgGlobalSize;	// Total Number Of Work Items
				size_t* ArgLocalSize;	// Work Items per Work Group
				size_t* ArgWorkSizeOffset;	// WorkSize Offset

				ArgNDRange->GetNDRange(&ArgDimensions, &ArgGlobalSize, &ArgLocalSize, &ArgWorkSizeOffset, Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error ArgNDRange->GetNDRange in CopyNDRange In: cl_NDRangeStruct!\n";
				}
				else
				{
					Dimensions = ArgDimensions;//By default it is 1
					GlobalWorkSize[0] = ArgGlobalSize[0];
					GlobalWorkSize[1] = ArgGlobalSize[1];
					GlobalWorkSize[2] = ArgGlobalSize[2];

					LocalWorkSize[0] = ArgLocalSize[0];
					LocalWorkSize[1] = ArgLocalSize[1];
					LocalWorkSize[2] = ArgLocalSize[2];

					WorkSizeOffset[0] = ArgWorkSizeOffset[0];
					WorkSizeOffset[1] = ArgWorkSizeOffset[1];
					WorkSizeOffset[2] = ArgWorkSizeOffset[2];
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error CopyNDRange() Failed in cl_MultiDevice_NDRangeStruct!\n";
			}
		}

		~cl_NDRangeStruct()
		{
			std::cout << "\n Destructing cl_NDRangeStruct!";
			Dimensions = 0;

			GlobalWorkSize[0] = 0;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = 0;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MultiDevice_NDRangeStruct
	{
	private:
		const unsigned int NumberOfDevices;
		cl_NDRangeStruct** MultiNDRange = nullptr;

	public:
		bool IsConstructionSuccesful = false;
		cl_MultiDevice_NDRangeStruct(unsigned int ArgTotalNumberOfDevices, bool& Issuccessful) : NumberOfDevices(ArgTotalNumberOfDevices)
		{
			IsConstructionSuccesful = false;
			Issuccessful = false;

			if (NumberOfDevices == 0)
			{
				std::cout << "\n Error : ArgTotalNumberOfDevices is 0, should be atleast be 1 In: cl_MultiDevice_NDRangeStruct!\n";
			}
			else
			{
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiNDRange, NumberOfDevices, sizeof(cl_NDRangeStruct*), Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error Allocating :" << NumberOfDevices * sizeof(cl_NDRangeStruct*) << " Byes Of Memory for MultiNDRange In: cl_MultiDevice_NDRangeStruct!\n";
				}
				else
				{
					for (int i = 0; i < NumberOfDevices; ++i)
					{
						MultiNDRange[i] = new cl_NDRangeStruct(Issuccessful);
						if (MultiNDRange[i] == nullptr)
						{
							Issuccessful = false;
						}
						else
						{
							if (!Issuccessful)
							{
								delete MultiNDRange[i];
							}
						}
						if (!Issuccessful)
						{
							std::cout << "\n Error Allocating :" << sizeof(cl_NDRangeStruct) << " Byes Of Memory for MultiNDRange[" << i << "] In: cl_MultiDevice_NDRangeStruct!\n";
							for (int j = 0; j < i; ++j)
							{
								delete MultiNDRange[j];
							}
							free(MultiNDRange);
							MultiNDRange = nullptr;
							break;
						}
					}
				}
			}		

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_MultiDevice_NDRangeStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		unsigned int GetNumberOfDevices() { return NumberOfDevices; }
		void GetNDRangeOfDevice(unsigned int DeviceNumber, cl_NDRangeStruct** ReturnNDRange, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetNDRangeOfDevice Without Constructing the struct In: cl_MultiDevice_NDRangeStruct!\n";
			}
			else
			{
				if (DeviceNumber < NumberOfDevices)
				{
					Issuccessful = true;
					*ReturnNDRange = MultiNDRange[DeviceNumber];					
				}
				else
				{
					std::cout << "\n Error Argument Number + 1' " << DeviceNumber + 1 << "' Is greater than the NumberOfDevices in GetNDRangeOfDevice In: cl_NDRangeStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetNDRangeOfDevice() Failed in cl_MultiDevice_NDRangeStruct!\n";
			}
		}

		void SetNDRangeOfDevice(unsigned int DeviceNumber, cl_NDRangeStruct* ArgNDRange, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRangeOfDevice Without Constructing the struct In: cl_MultiDevice_NDRangeStruct!\n";
			}
			else
			{
				if (ArgNDRange == nullptr)
				{
					std::cout << "\n Error ArgNDRange is nullptr in SetNDRangeOfDevice In: cl_NDRangeStruct!\n";
				}

				if (DeviceNumber < NumberOfDevices)
				{
					std::cout << "\n Error Argument Number + 1' " << DeviceNumber + 1 << "' Is greater than the NumberOfDevices in SetNDRangeOfDevice In: cl_NDRangeStruct!\n";
				}
				else
				{
					MultiNDRange[DeviceNumber]->CopyNDRange(ArgNDRange, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error MultiNDRange[DeviceNumber]->CopyNDRange() failed in SetNDRangeOfDevice In: cl_NDRangeStruct!\n";
					}
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetNDRangeOfDevice() Failed in cl_MultiDevice_NDRangeStruct!\n";
			}
		}

		~cl_MultiDevice_NDRangeStruct()
		{
			std::cout << "\n Destructing cl_MultiDevice_NDRangeStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					delete MultiNDRange[i];
				}
				free(MultiNDRange);
				MultiNDRange = nullptr;
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using 
	struct cl_KernelFunctionArgumentOrderListStruct
	{
	public:
		bool IsThisListUsable = false;

		const std::string KernelFunctionName;
		const unsigned int TotalNumberOfArugments;
		unsigned int NumberOfArgumentsSet = 0;
		int NumberOfReads = 0;
		int NumberOfWrites = 0;
		int NumberOfRead_And_Writes = 0;
		int NumberOfLocals = 0;
		int NumberOfPrivates = 0;

		cl_Memory_Type** KernelArgumentsInOrder = nullptr;// Manually Set the enum types or use 'FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(std::string ProgramKernelCode, std::string FunctionName, cl_KernelFunctionArgumentOrderListStruct& OrderedKernelArgumentList)' Function

		bool IsConstructionSuccesful = false;// NOTE: Never Change this Manualy not worth the time...
		cl_KernelFunctionArgumentOrderListStruct(const unsigned int ArgTotalNumberOfArugments, const std::string ArgKernelFunctionName, bool& Issuccessful) : TotalNumberOfArugments(ArgTotalNumberOfArugments), KernelFunctionName(ArgKernelFunctionName)
		{
			std::cout << "\n Constructing cl_KernelFunctionArgumentOrderListStruct!";

			IsThisListUsable = false;

			NumberOfArgumentsSet = 0;
			NumberOfReads = 0;
			NumberOfWrites = 0;
			NumberOfRead_And_Writes = 0;
			NumberOfLocals = 0;
			NumberOfPrivates = 0;

			Issuccessful = false;
			IsConstructionSuccesful = false;
			KernelArgumentsInOrder = nullptr;

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error Allocating :" << TotalNumberOfArugments * sizeof(cl_Memory_Type*) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";				
			}
			else
			{
				Issuccessful = true;// Since the Struct Does not have Issuccessful parameter
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					KernelArgumentsInOrder[i] = new cl_Memory_Type(cl_Memory_Type::Uninitialized_cl_Memory);
					if (KernelArgumentsInOrder[i] == nullptr)
					{
						Issuccessful = false;
					}
					else
					{
						if (*(KernelArgumentsInOrder[i]) != cl_Memory_Type::Uninitialized_cl_Memory)
						{
							delete KernelArgumentsInOrder[i];
						}
					}
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating " << sizeof(cl_Memory_Type) << " Byes Of Memory for KernelArgumentsInOrder[" << i << "] In KernelFunctionArgumentOrderListStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							delete KernelArgumentsInOrder[j];
						}
						free(KernelArgumentsInOrder);
						KernelArgumentsInOrder = nullptr;
						break;
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		cl_KernelFunctionArgumentOrderListStruct(const cl_KernelFunctionArgumentOrderListStruct* CopyStruct, bool& Issuccessful) : TotalNumberOfArugments(CopyStruct->TotalNumberOfArugments), KernelFunctionName(CopyStruct->KernelFunctionName)
		{
			std::cout << "\n Constructing cl_KernelFunctionArgumentOrderListStruct!";

			IsThisListUsable = false;

			NumberOfArgumentsSet = 0;
			NumberOfReads = 0;
			NumberOfWrites = 0;
			NumberOfRead_And_Writes = 0;
			NumberOfLocals = 0;
			NumberOfPrivates = 0;

			Issuccessful = false;
			IsConstructionSuccesful = false;
			KernelArgumentsInOrder = nullptr;

			if (!CopyStruct->IsConstructionSuccesful)
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
			}
			else
			{
				if (!CopyStruct->IsThisListUsable)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				}
				else
				{
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating :" << TotalNumberOfArugments * sizeof(cl_Memory_Type*) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";						
					}
					else
					{
						for (int i = 0; i < TotalNumberOfArugments; ++i)
						{
							KernelArgumentsInOrder[i] = new cl_Memory_Type(*((CopyStruct->KernelArgumentsInOrder)[i]));
							if (KernelArgumentsInOrder[i] == nullptr)
							{
								std::cout << "\n Error Allocating " << sizeof(cl_Memory_Type) << " Byes Of Memory for KernelArgumentsInOrder[" << i << "] In KernelFunctionArgumentOrderListStruct!\n";
								for (int j = 0; j < i; ++j)
								{
									delete KernelArgumentsInOrder[j];
								}
								free(KernelArgumentsInOrder);
								KernelArgumentsInOrder = nullptr;
								Issuccessful = false;
								break;
							}
						}
						if (Issuccessful)// For the safe of readability
						{
							IsThisListUsable = CopyStruct->IsThisListUsable;
							NumberOfArgumentsSet = CopyStruct->NumberOfArgumentsSet;
							NumberOfReads = CopyStruct->NumberOfReads;
							NumberOfWrites = CopyStruct->NumberOfWrites;
							NumberOfRead_And_Writes = CopyStruct->NumberOfRead_And_Writes;
							NumberOfLocals = CopyStruct->NumberOfLocals;
							NumberOfPrivates = CopyStruct->NumberOfPrivates;
						}
					}
				}
			}			

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}		

		void SetMemoryTypeOfArugment(const unsigned int ArgumentNumber, cl_Memory_Type MemoryType, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetMemoryTypeOfArugment Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
			else
			{
				if (MemoryType == cl_Memory_Type::Uninitialized_cl_Memory)
				{
					std::cout << "\n Error Trying to Set The MemoryType of the Argument Number'" << ArgumentNumber << "' to cl_Memory_Type::Uninitialized_cl_Memory In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
				}
				else
				{
					if (ArgumentNumber < TotalNumberOfArugments)
					{
						if (ArgumentNumber >= NumberOfArgumentsSet)
						{
							if (*(KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::Uninitialized_cl_Memory)
							{
								*(KernelArgumentsInOrder[ArgumentNumber]) = MemoryType;
								NumberOfArgumentsSet = NumberOfArgumentsSet + 1;
							}
						}
						else
						{
							switch (*(KernelArgumentsInOrder[ArgumentNumber]))
							{
							case cl_Memory_Type::CL_LOCALENUM:
								NumberOfLocals = NumberOfLocals - 1;
								break;

							case cl_Memory_Type::CL_PRIVATE:
								NumberOfPrivates = NumberOfPrivates - 1;
								break;

							case cl_Memory_Type::CL_READ_ONLY:
								NumberOfReads = NumberOfReads - 1;
								break;

							case cl_Memory_Type::CL_WRITE_ONLY:
								NumberOfWrites = NumberOfWrites - 1;
								break;

							case cl_Memory_Type::CL_READ_AND_WRITE:
								NumberOfRead_And_Writes = NumberOfRead_And_Writes - 1;
								break;

							default:
								std::cout << "\n CRITICAL ERROR UNDEFINED ENUM! In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
								break;
							}
							*(KernelArgumentsInOrder[ArgumentNumber]) = MemoryType;
						}

						switch (MemoryType)
						{
							case cl_Memory_Type::CL_LOCALENUM:
								NumberOfLocals = NumberOfLocals + 1;
								break;

							case cl_Memory_Type::CL_PRIVATE:
								NumberOfPrivates = NumberOfPrivates + 1;
								break;

							case cl_Memory_Type::CL_READ_ONLY:
								NumberOfReads = NumberOfReads + 1;
								break;

							case cl_Memory_Type::CL_WRITE_ONLY:
								NumberOfWrites = NumberOfWrites + 1;
								break;

							case cl_Memory_Type::CL_READ_AND_WRITE:
								NumberOfRead_And_Writes = NumberOfRead_And_Writes + 1;
								break;

							default:
								std::cout << "\n CRITICAL ERROR UNDEFINED ENUM! In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
								break;
						}

						if (NumberOfArgumentsSet == TotalNumberOfArugments)
						{
							IsThisListUsable = true;
						}
						Issuccessful = true;
					}
					else
					{
						std::cout << "\n Error Argument Number + 1' " << ArgumentNumber + 1 << "' Is greater than the TotalNumberOfArguments In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetMemoryTypeOfArugment() Failed in cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}

		//Redundant Getter Functions...
		/*void GetIsListUsable(bool& IsUsable)
		{
			IsUsable = IsThisListUsable && IsConstructionSuccesful;
		}
		void GetKernelFunctionName(std::string& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = "";
			if (IsConstructionSuccesful)
			{
				ReturnVal = KernelFunctionName;
			}
			else
			{
				std::cout << "\n Error Calling GetKernelFunctionName Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetTotalNumberOfArugments(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = TotalNumberOfArugments;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfArugments Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfReads(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfReads;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfReads Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfWrites(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfWrites;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfWrites Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfRead_And_Writes(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfRead_And_Writes;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfRead_And_Writes Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfLocals(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfLocals;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfLocals Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfPrivates(int& ReturnVal, bool& Issuccessful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfPrivates;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfPrivates Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetMemoryTypeOfArgument(unsigned int ArgumentNumber, cl_Memory_Type& TheMemoryType, bool& Issuccessful)
		{
			Issuccessful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetMemoryTypeOfArgument, The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_KernelFunctionArgumentOrderListStruct!\n";
				return;
			}
			else
			{
				if (!IsThisListUsable)
				{
					std::cout << "\n Error Calling GetMemoryTypeOfArgument,The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_KernelFunctionArgumentOrderListStruct!\n";
					return;
				}
			}
			if (ArgumentNumber < TotalNumberOfArugments)
			{
				TheMemoryType = *((KernelArgumentsInOrder)[ArgumentNumber]);
			}
			else
			{
				std::cout << "\n Error ArgumentNumber Exceeds the total number of Arguments Present! in GetMemoryTypeOfArgument In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}*/

		~cl_KernelFunctionArgumentOrderListStruct()
		{
			std::cout << "\n Destructing cl_KernelFunctionArgumentOrderListStruct!";
			if (IsConstructionSuccesful)
			{
				IsThisListUsable = false;
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					delete KernelArgumentsInOrder[i];
				}
				free(KernelArgumentsInOrder);
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MemoryStruct
	{
	private:
		const cl_kernel* TheKernel;
		const cl_uint KernelArgumentNumber;
		const cl_context* cl_ContextForThisArgument;
		const cl_command_queue* cl_CommandQueueForThisArgument;//One per device
		const cl_Memory_Type clMemory_Type_Of_Argument;
		bool DoesBufferAlreadyExist = false;
		cl_mem GlobalMemoryInDevice = nullptr;// Local not needed as it is inaccessible by host..
		void* COPY_OF_PrivateMemoryType = nullptr;
		size_t MemoryInDeviceTotalSizeInBytes = 0;//NOTE: for private pass the sizeof(variable_type)
		size_t MemoryInDevice_Occupied_SizeInBytes = 0;//NOTE: Actually 'MemoryInDeviceTotalSizeInBytes' occupies full space... but by 'MemoryInDevice_Occupied_SizeInBytes' I mean the memory YOU "use". I know this might sound confusing, but here is a simple example; Say you have a box that which can hold 20 pieces of bottles, but we only put in 10 pieces, meaning we used only 10 slot instead fully using 20 slots. 

		void FreeBuffer(bool& Issuccessful)
		{
			Issuccessful = false;

			if (DoesBufferAlreadyExist)
			{
				if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
					{
						if (COPY_OF_PrivateMemoryType != nullptr)
						{
							free(COPY_OF_PrivateMemoryType);
							COPY_OF_PrivateMemoryType = nullptr;
						}
						DoesBufferAlreadyExist = false;
						Issuccessful = true;
					}
					else
					{
						if (clMemory_Type_Of_Argument != cl_Memory_Type::CL_LOCALENUM)
						{
							cl_uint ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);// releasing Memory object every time this function is called	
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n ClError Code " << ClErrorResult << " : Releasing Memory On device In: cl_MemoryStruct!\n";
								std::cout << "\n Error clReleaseMemObject failed in FreeBuffer() Returning Early! in cl_MemoryStruct!\n";
							}
							else
							{
								DoesBufferAlreadyExist = false;
								Issuccessful = true;
								MemoryInDeviceTotalSizeInBytes = 0;
								MemoryInDevice_Occupied_SizeInBytes = 0;
							}
						}
						else
						{
							DoesBufferAlreadyExist = false;
							Issuccessful = true;
							MemoryInDeviceTotalSizeInBytes = 0;
							MemoryInDevice_Occupied_SizeInBytes = 0;
						}						
					}					
				}
				else
				{
					std::cout << "\n Error Trying to Releasing Uninitialized_cl_Memory Memory On device In BufferCreation In: cl_MemoryStruct!\n";
				}
			}
			else
			{
				//std::cout << "\n Error Buffer does not exist, unable to free buffer" << ": BufferCreation In: cl_MemoryStruct!\n";
				Issuccessful = true;
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error FreeBuffer() Failed in cl_MemoryStruct!\n";
			}
		}

		void BufferCreation(size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (BUFFER_CREATION_ONLY_SizeOfBuffer < 1)
			{
				std::cout << "\n Error Supplied Size of " << BUFFER_CREATION_ONLY_SizeOfBuffer << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data for Buffer Creation" << ": BufferCreation In: cl_MemoryStruct!\n";
			}
			else
			{
				FreeBuffer(Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error FreeBuffer() failed in BufferCreation In: cl_MemoryStruct!\n";
				}
				else
				{
					cl_int ClErrorResult;
					Issuccessful = false;

					switch (clMemory_Type_Of_Argument)
					{
						case cl_Memory_Type::CL_LOCALENUM:
						{
							//No Need for Buffer creation as this is a local memory...
							//if (MemoryInDeviceTotalSizeInBytes != MemoryInDevice_Occupied_SizeInBytes)
							//{
							//	std::cout << "\n Error CL_LOCALENUM MemoryInDeviceTotalSizeInBytes is not equal to MemoryInDevice_Occupied_SizeInBytes In BufferCreation In: cl_MemoryStruct!\n";
							//	std::cout << "NOTE: Local Memory occupies space regardless of MemoryInDevice_Occupied_SizeInBytes, So both variables should have the same value\n";
							//	return;
							//}
							MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
							MemoryInDevice_Occupied_SizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
							Issuccessful = true;
							DoesBufferAlreadyExist = true;
							break;
						}

						case cl_Memory_Type::CL_PRIVATE:
						{
							if ((MemoryInDeviceTotalSizeInBytes != 0) || (MemoryInDevice_Occupied_SizeInBytes != 0))
							{
								std::cout << "\n Error Trying to Change the Size of CL_PRIVATE Memory On device In BufferCreation In: cl_MemoryStruct!\n";
								break;
							}

							COPY_OF_PrivateMemoryType = calloc(BUFFER_CREATION_ONLY_SizeOfBuffer, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
							if (COPY_OF_PrivateMemoryType == nullptr)
							{
								std::cout << "\n Error Allocating" << BUFFER_CREATION_ONLY_SizeOfBuffer * sizeof(char) << "COPY_OF_PrivateMemoryType Variable In: cl_MemoryStruct!\n";
								break;
							}

							MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
							MemoryInDevice_Occupied_SizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
							Issuccessful = true;
							DoesBufferAlreadyExist = true;
							break;
						}

						case cl_Memory_Type::Uninitialized_cl_Memory:
						{
							std::cout << "\n Error Default 'Uninitialized_cl_Memory' Enum Passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE In BufferCreation In: cl_MemoryStruct!\n";
							MemoryInDeviceTotalSizeInBytes = 0;
							MemoryInDevice_Occupied_SizeInBytes = 0;
							break;
						}

						default://CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY And CL_MEM_READ_WRITE
						{
							if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY)
							{
								GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_READ_ONLY, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
							}
							else
							{
								if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY)
								{
									GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_WRITE_ONLY, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
								}
								else
								{
									if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE)
									{
										GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_READ_WRITE, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
									}
									else
									{
										std::cout << "\n Error Undefined Enum Passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE In BufferCreation In: cl_MemoryStruct!\n";
										break;
									}
								}
							}
							//GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, clMemory_Type_Of_Argument, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n ClError Code " << ClErrorResult << " : Creating Buffer On device In BufferCreation In: cl_MemoryStruct!\n";
								MemoryInDeviceTotalSizeInBytes = 0;
								MemoryInDevice_Occupied_SizeInBytes = 0;
								Issuccessful = false;
							}
							else
							{
								DoesBufferAlreadyExist = true;// Buffer Created so it exists
								ClErrorResult = clEnqueueMigrateMemObjects(*cl_CommandQueueForThisArgument, 1, &GlobalMemoryInDevice, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, NULL);
								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "\n CL_Error Code " << ClErrorResult << "  in BufferCreation In: cl_MemoryStruct!\n";									
									FreeBuffer(Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error FreeBuffer() failed in BufferCreation In: cl_MemoryStruct!\n";
									}
									Issuccessful = false;
								}
								else
								{
									//MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;// Same Value
									MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
									MemoryInDevice_Occupied_SizeInBytes = 0;									
									DoesBufferAlreadyExist = true;
									Issuccessful = true;									
								}
							}
							break;
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error BufferCreation() Failed in cl_MemoryStruct!";
			}
		}

	public:
		bool IsConstructionSuccesful = false;

		cl_MemoryStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* ArgTheKernel, const cl_uint ArgKernelArgumentNumber, bool& Issuccessful) : clMemory_Type_Of_Argument(ArgclMemory_Type_Of_Argument), cl_ContextForThisArgument(Argcl_ContextForThisArgument), cl_CommandQueueForThisArgument(Argcl_CommandQueueForThisArgument), TheKernel(ArgTheKernel), KernelArgumentNumber(ArgKernelArgumentNumber)
		{
			std::cout << "\n Constructing cl_MemoryStruct!";

			DoesBufferAlreadyExist = false;
			GlobalMemoryInDevice = NULL;
			COPY_OF_PrivateMemoryType = nullptr;
			MemoryInDeviceTotalSizeInBytes = 0;
			MemoryInDevice_Occupied_SizeInBytes = 0;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if ((TheKernel == nullptr) || (cl_ContextForThisArgument == nullptr) || (cl_CommandQueueForThisArgument == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Memory_Struct!";				
			}
			else
			{
				BufferCreation(1, Issuccessful);// No Need to use Issuccessful Here
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
			}
			else
			{
				DoesBufferAlreadyExist = true;
				IsConstructionSuccesful = true;
			}
		}

		//NOTE: THIS is just a copy of The Original cl_Memory_Struct, Meaning Both the copy and original Should not be used at the same time!
		cl_MemoryStruct(cl_MemoryStruct* CopyStruct, bool AlsoCopyBufferAddressAndSize, bool& Issuccessful) : clMemory_Type_Of_Argument(CopyStruct->clMemory_Type_Of_Argument), cl_ContextForThisArgument(CopyStruct->cl_ContextForThisArgument), cl_CommandQueueForThisArgument(CopyStruct->cl_CommandQueueForThisArgument), TheKernel(CopyStruct->TheKernel), KernelArgumentNumber(CopyStruct->KernelArgumentNumber)
		{
			std::cout << "\n Constructing cl_MemoryStruct!";

			DoesBufferAlreadyExist = false;
			GlobalMemoryInDevice = NULL;
			COPY_OF_PrivateMemoryType = nullptr;
			MemoryInDeviceTotalSizeInBytes = 0;
			MemoryInDevice_Occupied_SizeInBytes = 0;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if (CopyStruct == nullptr)
			{
				std::cout << "\n Error CopyStruct is nullptr In cl_MemoryStruct!\n";
			}
			else
			{
				if (AlsoCopyBufferAddressAndSize)
				{
					DoesBufferAlreadyExist = CopyStruct->DoesBufferAlreadyExist;
					GlobalMemoryInDevice = CopyStruct->GlobalMemoryInDevice;
					COPY_OF_PrivateMemoryType = CopyStruct->COPY_OF_PrivateMemoryType;
					MemoryInDeviceTotalSizeInBytes = CopyStruct->MemoryInDeviceTotalSizeInBytes;
					MemoryInDevice_Occupied_SizeInBytes = CopyStruct->MemoryInDevice_Occupied_SizeInBytes;
				}
				else
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)// Since we are copying the contents, CL_PRIVATE will have fixed size
					{
						MemoryInDeviceTotalSizeInBytes = CopyStruct->MemoryInDeviceTotalSizeInBytes;
						MemoryInDevice_Occupied_SizeInBytes = CopyStruct->MemoryInDevice_Occupied_SizeInBytes;
					}
				}
				Issuccessful = true;
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void MemoryAllocationOnDevice(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& Issuccessful)//Note: If MemorySize is 0, Then no memory is written on the device but memory is created, Note: for CreateOnlyOptionalLargerBufferSize to take effect pass a size larger than SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling MemoryAllocationOnDevice Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				cl_int ClErrorResult;

				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type < 1)
				{
					std::cout << "\n Error :Supplied Memory Size of " << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
				}
				else
				{
					if (OverWriteMemory)
					{
						if (BUFFER_CREATION_ONLY_SizeOfBuffer > 0)
						{
							std::cout << "\n Error :Trying To OverWrite Memory but the 'BUFFER_CREATION_ONLY_SizeOfBuffer' argument is not set to equal to 0 in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
						}
						else
						{
							if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
							{
								//if (MemoryTypeOfThisMemoryInDevice != TheMemoryType)
								//{
								//	std::cout << "\n Error Supplied MemoryType Does not Match with Type Of MemoryInDevice" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
								//	return;
								//}
								////else// This is unlikely to happen see the above two if stement, MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory, Then MemoryTypeOfThisMemoryInDevice != TheMemoryType
								////{
								////	if (TheMemoryType == cl_Memory_Type::Uninitialized_cl_Memory)
								////	{
								////		std::cout << "\n Error " << ClErrorResult << " : Default 'Uninitialized_cl_Memory' Enum passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE\n";
								////	}
								////}
								if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type <= MemoryInDeviceTotalSizeInBytes)
								{
									if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
									{
										if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != MemoryInDeviceTotalSizeInBytes)
										{
											std::cout << "\n Error :Trying to change the Size of Private Variable(NOTE: This is impossible to happen. Because size of variables type does not change...) in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
										}
										else
										{
											for (int i = 0; i < MemoryInDeviceTotalSizeInBytes; ++i)// Memccpy bad
											{
												((char*)COPY_OF_PrivateMemoryType)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
											}
											Issuccessful = true;
										}
									}
									else
									{
										if (clMemory_Type_Of_Argument != cl_Memory_Type::CL_LOCALENUM)// Uninitialized_cl_Memory Is imposible in this case, reason being if IsInitialized == true, then MemoryTypeOfThisMemoryInDevice is definitely not Uninitialized_cl_Memory
										{
											void* TempDataCarryHelper = calloc(SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
											if (TempDataCarryHelper == nullptr)
											{
												std::cout << "\n Error Allocating:" << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type * sizeof(char) << "TempDataCarryHelper Variable in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
											}
											else
											{
												if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
												{
													for (int i = 0; i < SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type; ++i)// Memccpy bad
													{
														((char*)TempDataCarryHelper)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
													}

													ClErrorResult = clEnqueueWriteBuffer(*cl_CommandQueueForThisArgument, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, TempDataCarryHelper, 0, NULL, NULL);
													free(TempDataCarryHelper);// Free the Data

													if (ClErrorResult != CL_SUCCESS)
													{
														std::cout << "\n CL_Error Code " << ClErrorResult << " in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
													}
													else
													{
														MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
														Issuccessful = true;
													}
												}
												else
												{
													std::cout << "\n Error :Trying To OverWrite Memory but 'SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type is Set to 0 in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
												}
											}											
										}
										else
										{
											if (PointerToMemoryToCopyFrom != nullptr)
											{
												std::cout << "\n Error :Trying To OverWrite Local Memory(This type can not be written or overwritten from host...) So Pass nullptr to the parameter 'PointerToMemoryToCopyFrom' in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
											}
											else
											{
												MemoryInDeviceTotalSizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
												MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
												Issuccessful = true;
											}
										}
									}
								}
								else
								{
									std::cout << "\n Error :Size Passed is larger than the Available buffer Space in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
								}
							}
							else
							{
								std::cout << "\n Error :Trying To OverWrite Uninitialized Memory" << ": OverWriting Buffer in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
							}
						}
					}
					else
					{
						if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > BUFFER_CREATION_ONLY_SizeOfBuffer)
						{
							std::cout << "\n Error :Supplied Size Of SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type Excedes the size maximum size specified by BUFFER_CREATION_ONLY_SizeOfBuffer in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
						}
						else
						{
							BufferCreation(BUFFER_CREATION_ONLY_SizeOfBuffer, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error :Buffer Creation Unsuccesful In MemoryAllocationOnDevice in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
							}
							else
							{
								Issuccessful = false;// Yes it is set to false So that it can be used for below code
								if ((clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE))
								{
									if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
									{
										ClErrorResult = clEnqueueWriteBuffer(*cl_CommandQueueForThisArgument, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, PointerToMemoryToCopyFrom, 0, NULL, NULL);
										if (ClErrorResult != CL_SUCCESS)
										{
											std::cout << "\n Error Code " << ClErrorResult << " in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
											FreeBuffer(Issuccessful);
											if (!Issuccessful)
											{
												std::cout << "\n Error FreeBuffer() failed in MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
											}
										}
										else
										{
											//Put the Buffer in Specified Device(GPU Or CPU)
											MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
											Issuccessful = true;
										}
									}
								}
								else
								{
									//Only Works for First time for the second and other times this section will not even execute...
									if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
									{
										for (int i = 0; i < SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type; ++i)// Memccpy bad
										{
											((char*)COPY_OF_PrivateMemoryType)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
										}
										Issuccessful = true;
									}//Local can get pass through here but no code is required for it...
									//else No need for else, as it is impossible for invalid or other enum type to get past through, unless a glitch occurs which is extremely unlikely...
								}
							}
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error MemoryAllocationOnDevice() Failed in cl_MemoryStruct!";
			}
		}

		void PassBufferToKernel(bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling PassBufferToKernel Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				if (!DoesBufferAlreadyExist)
				{
					std::cout << "\n Error :Kernel Argument Number of " << KernelArgumentNumber << " Does not have buffer created. in ReadBuffer In: cl_MemoryStruct!\n";
				}
				else
				{
					cl_int ClErrorResult = 0;
					if ((clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE))
					{
						ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, sizeof(cl_mem), &GlobalMemoryInDevice);
					}
					else
					{
						if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_LOCALENUM)
						{
							ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, MemoryInDeviceTotalSizeInBytes, NULL);
						}
						else
						{
							if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
							{
								ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, MemoryInDeviceTotalSizeInBytes, COPY_OF_PrivateMemoryType);
							}
							//else No need for else, as it is impossible for invalid or other enum type to get past through, unless a glitch occurs which is extremely unlikely...
						}
					}
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n CL_Error Code " << ClErrorResult << " : Passing Buffer To Kernel In PassBufferToKernel In: cl_MemoryStruct!\n";
					}
					else
					{
						Issuccessful = true;
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error PassBufferToKernel() Failed in cl_MemoryStruct!";
			}
		}

		void AllocateMemoryAndPassToKernel(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				if (UsePreviouslyAllocatedMemoryOnBuffer)
				{

					Issuccessful = true;
					if (!DoesBufferAlreadyExist)
					{
						std::cout << "\n Error :Kernel Argument Number of " << KernelArgumentNumber << " Does not have buffer created. in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
						Issuccessful = false;
					}					
					if (PointerToMemoryToCopyFrom != nullptr)
					{
						std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but PointerToMemoryToCopyFrom is not nullptr in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
						Issuccessful = false;
					}
					if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != 0)
					{
						std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type is set not to 0 in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
						Issuccessful = false;
					}
					if (BUFFER_CREATION_ONLY_SizeOfBuffer != 0)
					{
						std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but BUFFER_CREATION_ONLY_SizeOfBuffer is set not to 0 in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
						Issuccessful = false;
					}
					//Continue to PassBufferToKernel
				}
				else
				{
					MemoryAllocationOnDevice(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error :Memory Allocation On Device was Unsuccesful In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					}
				}

				if (Issuccessful)
				{
					PassBufferToKernel(Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error :Sending buffer to kernel failed In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					}
				}
			}
			
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error PassBufferToKernel() Failed in cl_MemoryStruct!";
			}
		}

		bool GetDoesBufferAlreadyExist() { return DoesBufferAlreadyExist; }

		void ReadBuffer(Essenbp::UnknownDataAndSizeStruct& RetreivedData, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling ReadBuffer Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				if (!DoesBufferAlreadyExist)
				{
					std::cout << "\n Error :Kernel Argument Number of " << KernelArgumentNumber << " Does not have buffer created. in ReadBuffer In: cl_MemoryStruct!\n";
				}
				else
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_LOCALENUM)
					{
						std::cout << "\n Error: Trying to Read Local in Memory ReadBuffer In: cl_MemoryStruct!\n";
						std::cout << "NOTE: Only Read,Write, ReadAndWrite Memory Be read! Local and Private can not be read\n";
					}
					else
					{
						if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
						{
							RetreivedData.CopyAndStoreData(COPY_OF_PrivateMemoryType, MemoryInDeviceTotalSizeInBytes, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error :Calling RetreivedData.CopyAndStoreData() failed in ReadBuffer In: cl_MemoryStruct!\n";
							}
							//std::cout << "\n Error: Trying to Read Private in Memory ReadBuffer In: cl_MemoryStruct!\n";
							//std::cout << "NOTE: Only Read,Write, ReadAndWrite Memory Be read! Local and Private can not be read\n";
							//Issuccessful = false;
						}
						else
						{
							RetreivedData.FreeAndResizeData(MemoryInDeviceTotalSizeInBytes, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error :Calling RetreivedData.FreeAndResizeData() failed in ReadBuffer In: cl_MemoryStruct!\n";
							}
							else
							{
								Issuccessful = false;
								cl_int ClErrorResult = clEnqueueReadBuffer(*cl_CommandQueueForThisArgument, GlobalMemoryInDevice, CL_TRUE, 0, MemoryInDeviceTotalSizeInBytes, RetreivedData.GetData(), 0, NULL, NULL);

								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "Error Code " << ClErrorResult << " : Reading Buffer For Kernel Argument Number of " << KernelArgumentNumber << "'in ReadBuffer In: cl_MemoryStruct!\n";
								}
								else
								{
									Issuccessful = true;
								}
							}
						}
					}
				}				
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error ReadBuffer() Failed in cl_MemoryStruct!";
			}
		}

		//NOTE: The newly interchanged Buffers are automatically passed to kernel!
		void InterchangeBufferWithinSameDevice(cl_MemoryStruct* PointerToBuffer, bool& Issuccessful)//PointerToBuffer = MultiBufferOnDevice[i]
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterChangeBufferWithinSameDevice Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				if (cl_CommandQueueForThisArgument != PointerToBuffer->cl_CommandQueueForThisArgument)
				{
					std::cout << "\n Error :cl_CommandQueueForThisArgument of this Does not match with PointerToBuffer->cl_CommandQueueForThisArgument in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
					std::cout << " NOTE: use InterchangeBufferWithinSameDevice when interchanging data with Same device\n";
					std::cout << " NOTE: use InterchangeBufferWithAnotherDevice when interchanging data with Another device\n";
				}
				else
				{
					if (clMemory_Type_Of_Argument == PointerToBuffer->clMemory_Type_Of_Argument)
					{
						cl_MemoryStruct* TEMP_COPY_BufferOne = nullptr;//It Will Hold Buffer Of This and Context of (*PointerToBuffer)
						cl_MemoryStruct* TEMP_COPY_BufferTwo = nullptr;//It Will Hold Buffer Of (*PointerToBuffer) and Context of this

						TEMP_COPY_BufferOne = new cl_MemoryStruct(PointerToBuffer, true, Issuccessful);
						if (TEMP_COPY_BufferOne == nullptr)
						{
							Issuccessful = false;
						}
						else
						{
							if (!Issuccessful)
							{
								delete TEMP_COPY_BufferOne;
							}
							else
							{
								TEMP_COPY_BufferOne->DoesBufferAlreadyExist =				DoesBufferAlreadyExist;
								TEMP_COPY_BufferOne->GlobalMemoryInDevice =					GlobalMemoryInDevice;
								TEMP_COPY_BufferOne->COPY_OF_PrivateMemoryType =			COPY_OF_PrivateMemoryType;
								TEMP_COPY_BufferOne->MemoryInDeviceTotalSizeInBytes =		MemoryInDeviceTotalSizeInBytes;
								TEMP_COPY_BufferOne->MemoryInDevice_Occupied_SizeInBytes =	MemoryInDevice_Occupied_SizeInBytes;
							}
						}

						if (!Issuccessful)
						{
							std::cout << "\n Error :Construction of TEMP_COPY_BufferOne failed in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
						}
						else
						{
							TEMP_COPY_BufferTwo = new cl_MemoryStruct(this, true, Issuccessful);
							if (TEMP_COPY_BufferTwo == nullptr)
							{
								Issuccessful = false;
								TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
								delete TEMP_COPY_BufferOne;
							}
							else
							{
								if (!Issuccessful)
								{
									delete TEMP_COPY_BufferTwo;// No need for this since the construction is unsuccesful...
									TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
									delete TEMP_COPY_BufferOne;
								}
								else
								{
									TEMP_COPY_BufferTwo->DoesBufferAlreadyExist =				PointerToBuffer->DoesBufferAlreadyExist;
									TEMP_COPY_BufferTwo->GlobalMemoryInDevice =					PointerToBuffer->GlobalMemoryInDevice;
									TEMP_COPY_BufferTwo->COPY_OF_PrivateMemoryType =			PointerToBuffer->COPY_OF_PrivateMemoryType;
									TEMP_COPY_BufferTwo->MemoryInDeviceTotalSizeInBytes =		PointerToBuffer->MemoryInDeviceTotalSizeInBytes;
									TEMP_COPY_BufferTwo->MemoryInDevice_Occupied_SizeInBytes =	PointerToBuffer->MemoryInDevice_Occupied_SizeInBytes;
								}
							}

							if (!Issuccessful)
							{
								std::cout << "\n Error :Construction of TEMP_COPY_BufferTwo failed in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
							}
							else
							{
								if (TEMP_COPY_BufferOne->DoesBufferAlreadyExist)
								{
									TEMP_COPY_BufferOne->PassBufferToKernel(Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error :TEMP_COPY_BufferOne->PassBufferToKernel failed in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
									}
									else
									{
										if (TEMP_COPY_BufferTwo->DoesBufferAlreadyExist)
										{
											TEMP_COPY_BufferTwo->PassBufferToKernel(Issuccessful);
										}
										if (!Issuccessful)
										{
											std::cout << "\n Error :TEMP_COPY_BufferTwo->PassBufferToKernel failed in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
										}
										else//When Everything is Succesfuly done
										{
											//Context Of One and Buffer of Two
											DoesBufferAlreadyExist = TEMP_COPY_BufferTwo->DoesBufferAlreadyExist;
											GlobalMemoryInDevice = TEMP_COPY_BufferTwo->GlobalMemoryInDevice;
											COPY_OF_PrivateMemoryType = TEMP_COPY_BufferTwo->COPY_OF_PrivateMemoryType;
											MemoryInDeviceTotalSizeInBytes = TEMP_COPY_BufferTwo->MemoryInDeviceTotalSizeInBytes;
											MemoryInDevice_Occupied_SizeInBytes = TEMP_COPY_BufferTwo->MemoryInDevice_Occupied_SizeInBytes;

											//Context Of Two and Buffer of One
											PointerToBuffer->DoesBufferAlreadyExist = TEMP_COPY_BufferOne->DoesBufferAlreadyExist;
											PointerToBuffer->GlobalMemoryInDevice = TEMP_COPY_BufferOne->GlobalMemoryInDevice;
											PointerToBuffer->COPY_OF_PrivateMemoryType = TEMP_COPY_BufferOne->COPY_OF_PrivateMemoryType;
											PointerToBuffer->MemoryInDeviceTotalSizeInBytes = TEMP_COPY_BufferOne->MemoryInDeviceTotalSizeInBytes;
											PointerToBuffer->MemoryInDevice_Occupied_SizeInBytes = TEMP_COPY_BufferOne->MemoryInDevice_Occupied_SizeInBytes;
										}
										
										//Freeing Memory
										TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
										delete TEMP_COPY_BufferOne;
										TEMP_COPY_BufferTwo->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
										delete TEMP_COPY_BufferTwo;// No need for this since the construction is unsuccesful...
									}
								}								
							}
						}
					}
					else
					{
						std::cout << "\n Error :clMemory_Type_Of_Argument of this Does not match with PointerToBuffer->clMemory_Type_Of_Argument in InterchangeBufferWithinSameDevice In: cl_MemoryStruct!\n";
					}
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithinSameDevice() Failed in cl_MemoryStruct!";
			}
		}

		//NOTE: The newly interchanged Buffers are automatically passed to kernel!
		void InterchangeBufferWithAnotherDevice(cl_MemoryStruct* PointerToBuffer, bool& Issuccessful)//PointerToBuffer = MultiBufferOnDevice[i]
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterchangeBufferWithAnotherDevice Without Constructing the struct In: cl_MemoryStruct!\n";
			}
			else
			{
				if (cl_CommandQueueForThisArgument == PointerToBuffer->cl_CommandQueueForThisArgument)
				{
					std::cout << "\n Error :cl_CommandQueueForThisArgument of this Matches with PointerToBuffer->cl_CommandQueueForThisArgument in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
					std::cout << " NOTE: use InterchangeBufferWithAnotherDevice when interchanging data with Another device\n";
					std::cout << " NOTE: use InterchangeBufferWithinSameDevice when interchanging data with Same device\n";
				}
				else
				{
					if (clMemory_Type_Of_Argument == PointerToBuffer->clMemory_Type_Of_Argument)
					{
						cl_MemoryStruct* TEMP_COPY_BufferOne = nullptr;
						cl_MemoryStruct* TEMP_COPY_BufferTwo = nullptr;

						TEMP_COPY_BufferOne = new cl_MemoryStruct(PointerToBuffer, true, Issuccessful);
						if (TEMP_COPY_BufferOne == nullptr)
						{
							Issuccessful = false;
						}
						else
						{
							if (!Issuccessful)
							{
								delete TEMP_COPY_BufferOne;
							}
							else
							{
								TEMP_COPY_BufferOne->DoesBufferAlreadyExist = DoesBufferAlreadyExist;
								TEMP_COPY_BufferOne->GlobalMemoryInDevice = GlobalMemoryInDevice;
								TEMP_COPY_BufferOne->COPY_OF_PrivateMemoryType = COPY_OF_PrivateMemoryType;
								TEMP_COPY_BufferOne->MemoryInDeviceTotalSizeInBytes = MemoryInDeviceTotalSizeInBytes;
								TEMP_COPY_BufferOne->MemoryInDevice_Occupied_SizeInBytes = MemoryInDevice_Occupied_SizeInBytes;
							}
						}

						if (!Issuccessful)
						{
							std::cout << "\n Error :Construction of TEMP_COPY_BufferOne failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
						}
						else
						{
							TEMP_COPY_BufferTwo = new cl_MemoryStruct(this, true, Issuccessful);
							if (TEMP_COPY_BufferTwo == nullptr)
							{
								Issuccessful = false;
								TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
								delete TEMP_COPY_BufferOne;
							}
							else
							{
								if (!Issuccessful)
								{
									TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
									delete TEMP_COPY_BufferOne;
									delete TEMP_COPY_BufferTwo;// No need for this since the construction is unsuccesful...
								}
								else
								{
									TEMP_COPY_BufferTwo->DoesBufferAlreadyExist = PointerToBuffer->DoesBufferAlreadyExist;
									TEMP_COPY_BufferTwo->GlobalMemoryInDevice = PointerToBuffer->GlobalMemoryInDevice;
									TEMP_COPY_BufferTwo->COPY_OF_PrivateMemoryType = PointerToBuffer->COPY_OF_PrivateMemoryType;
									TEMP_COPY_BufferTwo->MemoryInDeviceTotalSizeInBytes = PointerToBuffer->MemoryInDeviceTotalSizeInBytes;
									TEMP_COPY_BufferTwo->MemoryInDevice_Occupied_SizeInBytes = PointerToBuffer->MemoryInDevice_Occupied_SizeInBytes;
								}
							}

							if (!Issuccessful)
							{
								std::cout << "\n Error :Construction of TEMP_COPY_BufferTwo failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
							}
							else
							{
								if (TEMP_COPY_BufferOne->DoesBufferAlreadyExist)
								{
									TEMP_COPY_BufferOne->PassBufferToKernel(Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error :TEMP_COPY_BufferOne->PassBufferToKernel failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
									}
									else
									{
										cl_int ClErrorResult = clEnqueueMigrateMemObjects(*(TEMP_COPY_BufferOne->cl_CommandQueueForThisArgument), 1, &(TEMP_COPY_BufferOne->GlobalMemoryInDevice), CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, NULL);
										if (ClErrorResult != CL_SUCCESS)
										{
											std::cout << "\n CL_Error Code " << ClErrorResult << "  in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
											std::cout << "\n Error :clEnqueueMigrateMemObjects on TEMP_COPY_BufferOne failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
											Issuccessful = false;
										}
										else
										{
											if (TEMP_COPY_BufferTwo->DoesBufferAlreadyExist)
											{
												TEMP_COPY_BufferTwo->PassBufferToKernel(Issuccessful);
											}
											if (!Issuccessful)
											{
												std::cout << "\n Error :TEMP_COPY_BufferTwo->PassBufferToKernel failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
											}
											else
											{
												ClErrorResult = clEnqueueMigrateMemObjects(*(TEMP_COPY_BufferTwo->cl_CommandQueueForThisArgument), 1, &(TEMP_COPY_BufferTwo->GlobalMemoryInDevice), CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, NULL);
												if (ClErrorResult != CL_SUCCESS)
												{
													std::cout << "\n CL_Error Code " << ClErrorResult << "  in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
													std::cout << "\n Error :clEnqueueMigrateMemObjects on TEMP_COPY_BufferTwo failed in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
													Issuccessful = false;

													//Reseting to original Memory location!
													clEnqueueMigrateMemObjects(*cl_CommandQueueForThisArgument, 1, &(TEMP_COPY_BufferOne->GlobalMemoryInDevice), CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, NULL);
												}
												else//When Everything is Succesfuly done
												{
													//Context Of One and Buffer of Two
													DoesBufferAlreadyExist = TEMP_COPY_BufferTwo->DoesBufferAlreadyExist;
													GlobalMemoryInDevice = TEMP_COPY_BufferTwo->GlobalMemoryInDevice;
													COPY_OF_PrivateMemoryType = TEMP_COPY_BufferTwo->COPY_OF_PrivateMemoryType;
													MemoryInDeviceTotalSizeInBytes = TEMP_COPY_BufferTwo->MemoryInDeviceTotalSizeInBytes;
													MemoryInDevice_Occupied_SizeInBytes = TEMP_COPY_BufferTwo->MemoryInDevice_Occupied_SizeInBytes;

													//Context Of Two and Buffer of One
													PointerToBuffer->DoesBufferAlreadyExist = TEMP_COPY_BufferOne->DoesBufferAlreadyExist;
													PointerToBuffer->GlobalMemoryInDevice = TEMP_COPY_BufferOne->GlobalMemoryInDevice;
													PointerToBuffer->COPY_OF_PrivateMemoryType = TEMP_COPY_BufferOne->COPY_OF_PrivateMemoryType;
													PointerToBuffer->MemoryInDeviceTotalSizeInBytes = TEMP_COPY_BufferOne->MemoryInDeviceTotalSizeInBytes;
													PointerToBuffer->MemoryInDevice_Occupied_SizeInBytes = TEMP_COPY_BufferOne->MemoryInDevice_Occupied_SizeInBytes;
												}
											}
										}
									}
									//Freeing Memory
									TEMP_COPY_BufferOne->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
									delete TEMP_COPY_BufferOne;
									TEMP_COPY_BufferTwo->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
									delete TEMP_COPY_BufferTwo;// No need for this since the construction is unsuccesful...
								}
							}
						}
					}
					else
					{
						std::cout << "\n Error :clMemory_Type_Of_Argument of this Does not match with PointerToBuffer->clMemory_Type_Of_Argument in InterchangeBufferWithAnotherDevice In: cl_MemoryStruct!\n";
					}
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_MemoryStruct!";
			}
		}

		~cl_MemoryStruct()
		{
			std::cout << "\n Destructing cl_MemoryStruct!";
			if (IsConstructionSuccesful)
			{
				if (DoesBufferAlreadyExist)
				{
					FreeBuffer(IsConstructionSuccesful);//Temporarily using IsConstructionSuccesful boolean here //Does not matter if this fails because it can't be accesed anyway...					
				}
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_KernelSingleArgumentStruct
	{
	private:
		unsigned int BufferToUse = 0;
		unsigned int TotalNumberOfBuffers = 0;
		cl_MemoryStruct** MultiBufferOnDevice = nullptr;//Example: Multiple Buffers on GPU device(For each Kernel Argument)
		bool** IsBufferReady = nullptr;// This tells whether the buffer is allocated or not

	public:
		bool IsConstructionSuccesful = false;// NOTE: Do not change this manualy! unless you know what you are doing

		//Contructor
		cl_KernelSingleArgumentStruct(cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, cl_kernel* TheKernel, const cl_uint KernelArgumentNumber, bool& Issuccessful)
		{
			std::cout << "\n Constructing cl_KernelSingleArgumentStruct!";

			BufferToUse = 0;
			TotalNumberOfBuffers = 0;
			MultiBufferOnDevice = nullptr;
			MultiBufferOnDevice = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if ((Argcl_ContextForThisArgument == nullptr) || (Argcl_CommandQueueForThisArgument == nullptr) || (TheKernel == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_KernelSingleArgumentStruct!";
			}
			else
			{
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&IsBufferReady, 1, sizeof(bool*), Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error Allocating :" << sizeof(bool*) << " Byes Of Memory for IsBufferReady In cl_KernelSingleArgumentStruct!\n";
				}
				else
				{
					//Intial First Buffer
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiBufferOnDevice, 1, sizeof(cl_Memory_Type*), Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating :" << sizeof(cl_Memory_Type*) << " Byes Of Memory for MultiBufferOnDevice In cl_KernelSingleArgumentStruct!\n";
					}
					else
					{
						MultiBufferOnDevice[0] = new cl_MemoryStruct(ArgclMemory_Type_Of_Argument, Argcl_ContextForThisArgument, Argcl_CommandQueueForThisArgument, TheKernel, KernelArgumentNumber, Issuccessful);
						if (MultiBufferOnDevice[0] == nullptr)
						{
							Issuccessful = false;
							std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for MultiBufferOnDevice[0] In cl_KernelSingleArgumentStruct!\n";
						}
						else
						{
							if (!Issuccessful)
							{
								delete MultiBufferOnDevice[0];
								std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for MultiBufferOnDevice[0] In cl_KernelSingleArgumentStruct!\n";
							}
							else
							{
								IsBufferReady[0] = new bool(false);
								if (IsBufferReady[0] == nullptr)
								{
									std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for IsBufferReady[0] In cl_KernelSingleArgumentStruct!\n";
									Issuccessful = false;
								}
							}
						}					

						if (!Issuccessful)
						{							
							free(MultiBufferOnDevice);
							MultiBufferOnDevice = nullptr;
							free(IsBufferReady);
							IsBufferReady = nullptr;
						}
						else
						{
							TotalNumberOfBuffers = TotalNumberOfBuffers + 1;
						}
					}
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error Construction Failed cl_KernelSingleArgumentStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void AddBufferForThisArgument(bool& Issuccessful)
		{
			Issuccessful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AddBufferForThisArgument Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				cl_MemoryStruct** TEMP_COPY_MultiBufferOnDevice = nullptr;
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&TEMP_COPY_MultiBufferOnDevice, (TotalNumberOfBuffers + 1), sizeof(cl_Memory_Type*), Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error Allocating :" << sizeof(cl_Memory_Type*) * (TotalNumberOfBuffers + 1) << " Byes Of Memory for TEMP_COPY_MultiBufferOnDevice in AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
				}
				else
				{
					bool** TEMP_COPY_IsBufferReady = nullptr;
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&TEMP_COPY_IsBufferReady, 1, sizeof(bool*), Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating :" << sizeof(bool*) * (TotalNumberOfBuffers + 1) << " Byes Of Memory for TEMP_COPY_IsBufferReadyy In AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
					}
					else
					{
						for (int i = 0; i < TotalNumberOfBuffers; ++i)
						{
							TEMP_COPY_MultiBufferOnDevice[i] = new cl_MemoryStruct(MultiBufferOnDevice[i], true, Issuccessful);// Copies const variables, buffer location and sizes.						
							if (TEMP_COPY_MultiBufferOnDevice[i] == nullptr)
							{
								Issuccessful = false;
							}
							else
							{
								if (!Issuccessful)
								{
									delete TEMP_COPY_MultiBufferOnDevice[i];// Since the construction is not succesful this is safe to delete
								}
							}

							TEMP_COPY_IsBufferReady[i] = new bool(*(IsBufferReady[i]));
							if (TEMP_COPY_IsBufferReady[i] == nullptr)
							{
								Issuccessful = false;
							}

							if (!Issuccessful)
							{
								std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for TEMP_COPY_MultiBufferOnDevice[" << i << "] in AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
								for (int j = i; j < i; ++j)
								{
									TEMP_COPY_MultiBufferOnDevice[j]->IsConstructionSuccesful = false;//If This is not set to false then The original Struct will also be deleted...
									delete TEMP_COPY_MultiBufferOnDevice[j];
									delete TEMP_COPY_IsBufferReady[j];
								}
								break;
							}
						}
					}
					

					if (Issuccessful)
					{
						TEMP_COPY_MultiBufferOnDevice[TotalNumberOfBuffers] = new cl_MemoryStruct(MultiBufferOnDevice[0], false, Issuccessful);// Copies const variables and size only.
						if (TEMP_COPY_MultiBufferOnDevice[TotalNumberOfBuffers] == nullptr)
						{
							Issuccessful = false;
							std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for TEMP_COPY_MultiBufferOnDevice[" << TotalNumberOfBuffers << "] in AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
						}
						else
						{
							if (!Issuccessful)
							{
								delete TEMP_COPY_MultiBufferOnDevice[TotalNumberOfBuffers];
								std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for TEMP_COPY_MultiBufferOnDevice[" << TotalNumberOfBuffers << "] in AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
							}
						}

						TEMP_COPY_IsBufferReady[TotalNumberOfBuffers] = new bool(false);
						if (TEMP_COPY_IsBufferReady[TotalNumberOfBuffers] == nullptr)
						{
							Issuccessful = false;
						}
						else
						{
							std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for TEMP_COPY_IsBufferReady[" << TotalNumberOfBuffers << "] in AddBufferForThisArgument In cl_KernelSingleArgumentStruct!\n";
						}

						if (Issuccessful)
						{
							TotalNumberOfBuffers = TotalNumberOfBuffers + 1;
							free(MultiBufferOnDevice);
							free(IsBufferReady);
							MultiBufferOnDevice = TEMP_COPY_MultiBufferOnDevice;
							IsBufferReady = TEMP_COPY_IsBufferReady;
						}

					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error AddBufferForThisArgument() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void AllocateMemoryAndPassToKernel(unsigned int BufferNumber, void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					MultiBufferOnDevice[BufferNumber]->AllocateMemoryAndPassToKernel(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, UsePreviouslyAllocatedMemoryOnBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error :MultiBufferOnDevice[BufferNumber]->AllocateMemoryAndPassToKernel Failed in AllocateMemoryAndPassToKernel In: cl_KernelSingleArgumentStruct!\n";
					}
					else
					{
						BufferToUse = BufferNumber;
						*(IsBufferReady[BufferNumber]) = true;//SetBufferToUse(BufferNumber,Issuccessful);Both are same
					}
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in AllocateMemoryAndPassToKernel In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error AllocateMemoryAndPassToKernel() Failed in cl_KernelSingleArgumentStruct!";
			}
		}
		
		void RetreiveDataFromBuffer(unsigned int BufferNumber, Essenbp::UnknownDataAndSizeStruct& ReteivedData, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RetreiveDataFromBuffer Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					MultiBufferOnDevice[BufferNumber]->ReadBuffer(ReteivedData, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_MemoryStruct::ReadBuffer() Failed in RetreiveDataFromBuffer In: cl_KernelSingleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in InterchangeBufferWithinSameDevice In: cl_KernelSingleArgumentStruct!\n";
				}				
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RetreiveDataFromBuffer() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void InterchangeBufferWithinSameDevice(unsigned int BufferNumber, cl_MemoryStruct* PointerToBuffer, bool&Issuccessful)//PointerToBuffer = MultiBufferOnDevice[i]
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterChangeBufferWithinSameDevice Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					MultiBufferOnDevice[BufferNumber]->InterchangeBufferWithinSameDevice(PointerToBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_MemoryStruct::InterchangeBufferWithinSameDevice() Failed in InterchangeBufferWithinSameDevice In: cl_KernelSingleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in InterchangeBufferWithinSameDevice In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithinSameDevice() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void InterchangeBufferWithAnotherDevice(unsigned int BufferNumber, cl_MemoryStruct* PointerToBuffer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterchangeBufferWithAnotherDevice Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					MultiBufferOnDevice[BufferNumber]->InterchangeBufferWithAnotherDevice(PointerToBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_MemoryStruct::InterchangeBufferWithAnotherDevice() Failed in InterchangeBufferWithinSameDevice In: cl_KernelSingleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in InterchangeBufferWithAnotherDevice In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void IsBufferReadyForUse(unsigned int BufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling GetPointerToBufferPointer Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					Issuccessful = MultiBufferOnDevice[BufferNumber]->GetDoesBufferAlreadyExist();
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in IsBufferReadyForUse In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error IsBufferReadyForUse() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void GetTotalNumberOfBuffers(unsigned int& ArgTotalNumberOfBuffers, bool& Issuccessful)
		{ 	
			if (!IsConstructionSuccesful)
			{
				Issuccessful = false;
				std::cout << "\n Error :Calling GetTotalNumberOfBuffers Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error GetTotalNumberOfBuffers() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				Issuccessful = true;
				ArgTotalNumberOfBuffers = TotalNumberOfBuffers;
			}
		}

		void GetBufferNumberBeingUsed(unsigned int& ArgBufferToUse, bool& Issuccessful)
		{
			if (!IsConstructionSuccesful)
			{
				Issuccessful = false;
				std::cout << "\n Error :Calling SetBufferToUse Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error GetBufferNumberBeingUsed() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				Issuccessful = true;
				ArgBufferToUse = BufferToUse;
			}
		}

		void GetPointerToBufferPointer(unsigned int BufferNumber, cl_MemoryStruct** ReturnPointerToBufferPointer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling GetPointerToBufferPointer Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
			}
			else
			{
				if (BufferNumber < TotalNumberOfBuffers)
				{
					if(ReturnPointerToBufferPointer != nullptr)
					{
						Issuccessful = true;
						*ReturnPointerToBufferPointer = MultiBufferOnDevice[BufferNumber];
					}
					else
					{
						std::cout << "\n Error :ReturnPointerToBufferPointer is nullptr In: cl_KernelSingleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :BufferNumber + 1 exceeds the total number of buffers for this argument... in GetPointerToBufferPointer In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetPointerToBufferPointer() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		void SetBufferToUse(unsigned int ArgBufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling SetBufferToUse Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error SetBufferToUse() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				if (ArgBufferNumber < TotalNumberOfBuffers)
				{
					Issuccessful = true;
					BufferToUse = ArgBufferNumber;
				}
				else
				{
					std::cout << "\n Error :ArgBufferNumber + 1 exceeds the total number of buffers for this argument... in SetBufferToUse In: cl_KernelSingleArgumentStruct!\n";
				}				
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetBufferToUse() Failed in cl_KernelSingleArgumentStruct!";
			}
		}

		//Destructor
		~cl_KernelSingleArgumentStruct()
		{
			std::cout << "\n Destructing cl_KernelSingleArgumentStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < TotalNumberOfBuffers; ++i)
				{
					delete IsBufferReady[i];
					delete MultiBufferOnDevice[i];
				}
				free(IsBufferReady);
				free(MultiBufferOnDevice);
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_KernelMultipleArgumentStruct
	{
	private:
		cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here
		const cl_KernelFunctionArgumentOrderListStruct* OrderedListOfArugments;
		const cl_command_queue* cl_CommandQueueForThisKernel;
		cl_kernel ThisKernel = NULL;

	public:
		bool IsConstructionSuccesful = false;// NOTE: Memory Leaks bad, so do not change this manualy...
		cl_KernelMultipleArgumentStruct(const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* cl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, cl_program* BuiltClProgramContainingTheSpecifiedFunctions, bool& Issuccessful) : OrderedListOfArugments(ArgOrderedListOfArugments), cl_CommandQueueForThisKernel(Argcl_CommandQueueForThisKernel)
		{
			std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";

			SingleKernelFunctionMultiArgumentsArray = nullptr;

			Issuccessful = false;
			IsConstructionSuccesful = false;// Yes this is set to false

			if ((ArgOrderedListOfArugments == nullptr) || (cl_ContextForThisKernel == nullptr) || (cl_CommandQueueForThisKernel == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_KernelMultipleArgumentStruct!";
			}
			else
			{
				if (OrderedListOfArugments != nullptr)
				{
					if (OrderedListOfArugments->IsConstructionSuccesful)
					{
						if (OrderedListOfArugments->IsThisListUsable)
						{
							cl_int ClErrorResult = 0;
							ThisKernel = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunctions, OrderedListOfArugments->KernelFunctionName.c_str(), &ClErrorResult);
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n CLError " << ClErrorResult << " : Kernel Creation Failed in cl_KernelMultipleArgumentStruct!\n";
							}
							else
							{
								Essenbp::Malloc_PointerToArrayOfPointers((void***)&SingleKernelFunctionMultiArgumentsArray, OrderedListOfArugments->TotalNumberOfArugments, sizeof(cl_KernelSingleArgumentStruct*), Issuccessful);
								if (!Issuccessful)
								{
									std::cout << "\n Error Allocating " << (OrderedListOfArugments->TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!\n";
									Issuccessful = false;
									ClErrorResult = clReleaseKernel(ThisKernel);
									if (ClErrorResult != CL_SUCCESS)
									{
										std::cout << "\n CLError " << ClErrorResult << " : Releasing Kernel: " << "in cl_KernelMultipleArgumentStruct!\n";
									}
								}
								else
								{
									for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
									{
										SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct(*(OrderedListOfArugments->KernelArgumentsInOrder[i]), cl_ContextForThisKernel, cl_CommandQueueForThisKernel, &ThisKernel, i, Issuccessful);
										if (SingleKernelFunctionMultiArgumentsArray[i] == nullptr)
										{
											Issuccessful = false;
										}
										else
										{
											if (!Issuccessful)
											{
												delete SingleKernelFunctionMultiArgumentsArray[i];
											}
										}
										if (!Issuccessful)
										{
											std::cout << "\n Error Allocating :" << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray[" << i << "] In: cl_KernelMultipleArgumentStruct!\n";
											for (int j = 0; j < i; ++j)
											{
												delete SingleKernelFunctionMultiArgumentsArray[j];
											}
											free(SingleKernelFunctionMultiArgumentsArray);
											SingleKernelFunctionMultiArgumentsArray = nullptr;
											ClErrorResult = clReleaseKernel(ThisKernel);
											if (ClErrorResult != CL_SUCCESS)
											{
												std::cout << "\n CLError " << ClErrorResult << " : Releasing Kernel: " << "in cl_KernelMultipleArgumentStruct!\n";
											}
											break;
										}
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error The OrderedListOfArugments Is nullptr, So Is Unusable in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
				}
			}

			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
				return;
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void AllocateMemoryAndPassToKernel(unsigned int const KernelArgumentNumber, unsigned int BufferNumber, void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& Issuccessful)
		{
			Issuccessful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
			}
			else
			{
				if (KernelArgumentNumber >= OrderedListOfArugments->TotalNumberOfArugments)
				{
					std::cout << "\n Error KernelArgumentNumber + 1 Exceeds the Total Number Of Arugments Present! in AllocateMemoryAndPassToKernel In: cl_KernelMultipleArgumentStruct!\n";
				}
				else
				{
					SingleKernelFunctionMultiArgumentsArray[KernelArgumentNumber]->AllocateMemoryAndPassToKernel(BufferNumber, PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, UsePreviouslyAllocatedMemoryOnBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error :SingleKernelFunctionMultiArgumentsArray[" << KernelArgumentNumber << "] Failed in AllocateMemoryAndPassToKernel In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error AllocateMemoryAndPassToKernel() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void RunKernel(cl_NDRangeStruct* NDRange, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling RunKernel Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
			}
			else 
			{
				unsigned int BufferToUse = 0;
				Issuccessful = true;
				for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
				{
					SingleKernelFunctionMultiArgumentsArray[i]->GetBufferNumberBeingUsed(BufferToUse, Issuccessful);
					if(!Issuccessful)
					{
						std::cout << "\n Error :SingleKernelFunctionMultiArgumentsArray["<<i<<"]->GetBufferNumberBeingUsed() Failed in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
						break;
					}
					else
					{
						SingleKernelFunctionMultiArgumentsArray[i]->IsBufferReadyForUse(BufferToUse, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error :Kernel Is Not ready for use, Pass AllocateMemoryAndPassToKernel() buffer of Kernel Argument " << i << "Since it Does not Exist. in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
							break;
						}
					}
				}

				if (Issuccessful)
				{
					Issuccessful = false;
					if (NDRange == nullptr)
					{
						std::cout << "\n Error :The Argument NDRange is nullptr in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
						Issuccessful = false;
					}
					else
					{
						cl_uint Dimensions = 0;//By default it is 1
						size_t* GlobalWorkSize = nullptr;	// Total Number Of Work Items
						size_t* LocalWorkSize = nullptr;	// Work Items per Work Group
						size_t* WorkSizeOffset = nullptr;	// WorkSize Offset

						NDRange->GetNDRange(&Dimensions, &GlobalWorkSize, &LocalWorkSize, &WorkSizeOffset, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error :NDRange->GetNDRange() Failed in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
						}
						else//Run the Kernel
						{
							cl_int ClErrorResult = 0;
							if (WorkSizeOffset == nullptr)
							{
								if (LocalWorkSize == nullptr)
								{
									ClErrorResult = clEnqueueNDRangeKernel(*cl_CommandQueueForThisKernel, ThisKernel, Dimensions, NULL, GlobalWorkSize, NULL, 0, NULL, NULL);
								}
								else
								{
									ClErrorResult = clEnqueueNDRangeKernel(*cl_CommandQueueForThisKernel, ThisKernel, Dimensions, NULL, GlobalWorkSize, LocalWorkSize, 0, NULL, NULL);
								}
							}
							else
							{
								if (LocalWorkSize == nullptr)
								{
									ClErrorResult = clEnqueueNDRangeKernel(*cl_CommandQueueForThisKernel, ThisKernel, Dimensions, WorkSizeOffset, GlobalWorkSize, NULL, 0, NULL, NULL);
								}
								else
								{
									ClErrorResult = clEnqueueNDRangeKernel(*cl_CommandQueueForThisKernel, ThisKernel, Dimensions, WorkSizeOffset, GlobalWorkSize, LocalWorkSize, 0, NULL, NULL);
								}
							}
							
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n CL_Error Code " << ClErrorResult << " : clEnqueueNDRangeKernel in RunKernel In: MemoryAllocationOnDevice In: cl_KernelMultipleArgumentStruct!\n";
								Issuccessful = false;
							}
						}
					}
				}
			}			

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RunKernel() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void RetreiveDataFromKernel(unsigned int ArgumentNumber, unsigned int BufferNumber, Essenbp::UnknownDataAndSizeStruct& ReteivedData, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RetreiveDataFromBuffer Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
			}
			{
				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->RetreiveDataFromBuffer(BufferNumber, ReteivedData, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::RetreiveDataFromBuffer() Failed in RetreiveDataFromBuffer In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error ArgumentNumber Exceeds the Total Number Of Kernel Arguments Present in the Kernel '" << OrderedListOfArugments->KernelFunctionName << "'! in RetreiveDataFromBuffer In: cl_KernelMultipleArgumentStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RetreiveDataFromBuffer() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void InterchangeBufferWithinSameDevice(unsigned int ArgumentNumber, unsigned int BufferNumber, cl_MemoryStruct* PointerToBuffer, bool& Issuccessful)//PointerToBuffer = MultiBufferOnDevice[i]
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterChangeBufferWithinSameDevice Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
			}
			else
			{

				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->InterchangeBufferWithinSameDevice(BufferNumber, PointerToBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::InterchangeBufferWithinSameDevice() Failed in InterchangeBufferWithinSameDevice In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error ArgumentNumber Exceeds the Total Number Of Kernel Arguments Present in the Kernel '" << OrderedListOfArugments->KernelFunctionName << "'! in InterchangeBufferWithinSameDevice In: cl_KernelMultipleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithinSameDevice() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void InterchangeBufferWithAnotherDevice(unsigned int ArgumentNumber, unsigned int BufferNumber, cl_MemoryStruct* PointerToBuffer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterchangeBufferWithAnotherDevice Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
			}
			else
			{
				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->InterchangeBufferWithAnotherDevice(BufferNumber, PointerToBuffer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::InterchangeBufferWithAnotherDevice() Failed in InterchangeBufferWithAnotherDevice In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error ArgumentNumber Exceeds the Total Number Of Kernel Arguments Present in the Kernel '" << OrderedListOfArugments->KernelFunctionName << "'! in InterchangeBufferWithAnotherDevice In: cl_KernelMultipleArgumentStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void GetBufferNumberBeingUsed(unsigned int ArgumentNumber, unsigned int& ArgBufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling GetBufferNumberBeingUsed Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error GetBufferNumberBeingUsed() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->GetBufferNumberBeingUsed(ArgBufferNumber, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::GetBufferNumberBeingUsed() Failed in GetBufferNumberBeingUsed In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :ArgumentNumber + 1 exceeds the total number of arguments for this kernel.. in GetBufferNumberBeingUsed In: cl_KernelSingleArgumentStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetBufferNumberBeingUsed() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		void GetPointerToBufferPointer(unsigned int ArgumentNumber, unsigned int BufferNumber, cl_MemoryStruct** ReturnPointerToBufferPointer, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling GetPointerToBufferPointer Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error GetPointerToBufferPointer() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->GetPointerToBufferPointer(BufferNumber, ReturnPointerToBufferPointer, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::GetPointerToBufferPointer() Failed in GetPointerToBufferPointer In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :ArgumentNumber + 1 exceeds the total number of arguments for this kernel.. in GetPointerToBufferPointer In: cl_KernelSingleArgumentStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetPointerToBufferPointer() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		//NOTE: By Default Buffer 0 Will be used, If New buffer is allocated and sent to kernel then that buffer will be used
		void SetBufferToUse(unsigned int ArgumentNumber, unsigned int ArgBufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling SetBufferToUse Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error SetBufferToUse() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				if (ArgumentNumber < OrderedListOfArugments->TotalNumberOfArugments)
				{
					SingleKernelFunctionMultiArgumentsArray[ArgumentNumber]->SetBufferToUse(ArgBufferNumber, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelSingleArgumentStruct::SetBufferToUse() Failed in SetBufferToUse In: cl_KernelMultipleArgumentStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :ArgumentNumber + 1 exceeds the total number of arguments for this kernel.. in SetBufferToUse In: cl_KernelSingleArgumentStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_KernelMultipleArgumentStruct!";
			}
		}

		~cl_KernelMultipleArgumentStruct()
		{
			std::cout << "\n Destructing cl_KernelMultipleArgumentStruct!";
			if (IsConstructionSuccesful)
			{
				cl_int ClErrorResult = clReleaseKernel(ThisKernel);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing Kernel: " << "in cl_KernelMultipleArgumentStruct!\n";
				}
				if (SingleKernelFunctionMultiArgumentsArray != nullptr)
				{
					for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
					{
						delete SingleKernelFunctionMultiArgumentsArray[i];
					}
					free(SingleKernelFunctionMultiArgumentsArray);
				}
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MultiKernelFunction_MultiDeviceStruct
	{
		const unsigned int NumberOfDevices;
		const std::string KernelFunctionName;
		const cl_context* cl_ContextForThisKernel;
		const cl_PerDeviceValuesStruct* DeviceValuesForOneDevice;
		cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray = nullptr;

		bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!

		//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
		//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
		cl_MultiKernelFunction_MultiDeviceStruct(const unsigned int ArgNumberOfDevices, const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* Argcl_ContextForThisKernel, const cl_PerDeviceValuesStruct* ArgDeviceValuesForOneDevice,
			cl_program* BuiltClProgramContainingTheSpecifiedFunctions, bool& Issuccessful) : NumberOfDevices(ArgNumberOfDevices), KernelFunctionName(ArgOrderedListOfArugments->KernelFunctionName), cl_ContextForThisKernel(Argcl_ContextForThisKernel), DeviceValuesForOneDevice(ArgDeviceValuesForOneDevice)
		{
			std::cout << "\n Constructing cl_MultiKernelFunction_MultiDeviceStruct!";

			MultiDeviceKernelArgumentsArray = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if ((ArgOrderedListOfArugments == nullptr) || (cl_ContextForThisKernel == nullptr) || (DeviceValuesForOneDevice == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_MultiKernelFunction_MultiDeviceStruct!";
			}
			else
			{
				if (!ArgOrderedListOfArugments->IsConstructionSuccesful)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				}
				else
				{
					if (!ArgOrderedListOfArugments->IsThisListUsable)
					{
						std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
					}
					else
					{
						if (NumberOfDevices > 0)
						{
							Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiDeviceKernelArgumentsArray, NumberOfDevices, sizeof(cl_KernelMultipleArgumentStruct*), Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_MultiKernelFunction_MultiDeviceStruct!\n";
							}
							else
							{
								for (int i = 0; i < NumberOfDevices; ++i)
								{
									MultiDeviceKernelArgumentsArray[i] = new cl_KernelMultipleArgumentStruct(ArgOrderedListOfArugments, cl_ContextForThisKernel, &DeviceValuesForOneDevice->DeviceClCommandQueue, BuiltClProgramContainingTheSpecifiedFunctions, Issuccessful);
									if (MultiDeviceKernelArgumentsArray[i] == nullptr)
									{
										Issuccessful = false;
									}
									else
									{
										if (!Issuccessful)
										{
											delete MultiDeviceKernelArgumentsArray[i];
										}
									}
									if (!Issuccessful)
									{
										std::cout << "\n Error Allocating :" << sizeof(cl_KernelMultipleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[" << i << "] In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
										for (int j = 0; j < i; ++j)
										{
											delete MultiDeviceKernelArgumentsArray[j];
										}
										free(MultiDeviceKernelArgumentsArray);
										MultiDeviceKernelArgumentsArray = nullptr;
										break;
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error The Number Of Devices Is Set to Zero In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
						}
					}
				}
			}
			

			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStruct!";
				return;
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void RunKernel(unsigned int ArgDeviceNumber, cl_NDRangeStruct* NDRange, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling RunKernel Without Constructing the struct In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
			}
			else
			{
				if (ArgDeviceNumber < NumberOfDevices)
				{
					if (NDRange == nullptr)
					{
						std::cout << "\n Error :The Argument NDRange is nullptr in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
					}
					else
					{
						MultiDeviceKernelArgumentsArray[ArgDeviceNumber]->RunKernel(NDRange, Issuccessful);
					}
					if (!Issuccessful)
					{
						std::cout << "\n Error :cl_KernelMultipleArgumentStruct::RunKernel Failed in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error :ArgDeviceNumber Exceds the total number of Devices in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RunKernel() Failed in cl_MultiKernelFunction_MultiDeviceStruct!";
			}
		}

		~cl_MultiKernelFunction_MultiDeviceStruct()
		{
			std::cout << "\n Destructing cl_MultiKernelFunction_MultiDeviceStruct!";
			if (IsConstructionSuccesful)
			{
				if (MultiDeviceKernelArgumentsArray != nullptr)
				{
					for (int i = 0; i < NumberOfDevices; ++i)
					{
						delete MultiDeviceKernelArgumentsArray[i];
					}
					free(MultiDeviceKernelArgumentsArray);
				}
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Use this to manually add specific functions to the constructor of 'cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct', Also Call Constructor Before Using
	struct cl_KernelFunctionsStruct
	{
	public:
		cl_KernelFunctionArgumentOrderListStruct** OpenCL_KernelFunctions = nullptr;
		const unsigned int TotalNumberOfFuctions;
		bool IsAllFunctionsNameAndTotalArgumentSet = false;
		bool IsAllFunctionsUsable = false;
		unsigned int TotalNumberOfFunctionsNameAndTotalArgumentSet = 0;
		unsigned int TotalNumberOfFunctionsUsable = 0;

	public:
		bool IsConstructionSuccesful = false;
		cl_KernelFunctionsStruct(unsigned int NumberOfFunctionsToAdd, bool& Issuccessful) : TotalNumberOfFuctions(NumberOfFunctionsToAdd)
		{
			std::cout << "\n Constructing cl_KernelFunctionsStruct!";

			OpenCL_KernelFunctions = nullptr;
			IsAllFunctionsNameAndTotalArgumentSet = false;
			IsAllFunctionsUsable = false;
			TotalNumberOfFunctionsNameAndTotalArgumentSet = 0;
			TotalNumberOfFunctionsUsable = 0;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&OpenCL_KernelFunctions, NumberOfFunctionsToAdd, sizeof(cl_KernelFunctionArgumentOrderListStruct*), Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error Allocating " << (NumberOfFunctionsToAdd * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for VariableToInitialize In ManualInitializeKernelFunctionList In: cl_KernelFunctionsStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionsStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void SetTheNameAndNumberOfArgumentsForNextKernelFunction(std::string FunctionName, unsigned int TotalNumberOfArgumentsForTheFunction, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetTheNameAndNumberOfArgumentsForNextKernelFunction Without Constructing the struct In: cl_KernelFunctionsStruct!\n";
			}
			else
			{
				if (TotalNumberOfFunctionsNameAndTotalArgumentSet < TotalNumberOfFuctions)
				{
					if (OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] != nullptr)
					{
						delete OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet];
						OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] = nullptr;
					}
					OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] = new cl_KernelFunctionArgumentOrderListStruct(TotalNumberOfArgumentsForTheFunction, FunctionName, Issuccessful);

					if (OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] == nullptr)
					{
						Issuccessful = false;
					}
					else
					{
						if (!Issuccessful)
						{
							delete OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet];
							OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] = nullptr;
						}
					}
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating " << (sizeof(cl_KernelFunctionArgumentOrderListStruct)) << " Byes Of Memory for OpenCL_KernelFunctions[" << TotalNumberOfFunctionsNameAndTotalArgumentSet << "] in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";						
					}
				}
				else
				{
					std::cout << "\n Error Already Set the Name and Total Number of Arugments of all functions, ";
					if (IsAllFunctionsUsable)
					{
						std::cout << "Also all the functions are Fully Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
					}
					else
					{
						if (TotalNumberOfFunctionsUsable == 0)
						{
							std::cout << "But None Of functions Are Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
						}
						else
						{
							std::cout << "But only " << TotalNumberOfFunctionsUsable << "Number Of functions Are Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetTheNameAndNumberOfArgumentsForNextKernelFunction() Failed in cl_KernelFunctionsStruct!";
			}
		}

		void SetMemoryTypeOfArugment(const unsigned int FunctionNumber, const unsigned int ArgumentNumber, cl_Memory_Type MemoryType, bool& Issuccessful)//NOTE: Function Number Starts from 0 To (TotalFunctions - 1), Example: For the first Function Use 0, for the Second use 1 and so on
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetMemoryTypeOfArugment Without Constructing the struct In: cl_KernelFunctionsStruct!\n";
				return;
			}
			else
			{
				if (ArgumentNumber < OpenCL_KernelFunctions[FunctionNumber]->TotalNumberOfArugments)
				{
					if (OpenCL_KernelFunctions[FunctionNumber]->IsThisListUsable)
					{
						OpenCL_KernelFunctions[FunctionNumber]->SetMemoryTypeOfArugment(ArgumentNumber, MemoryType, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error The Function cl_KernelFunctionArgumentOrderListStruct:SetMemoryTypeOfArugment failed in SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
						}
					}
					else
					{
						OpenCL_KernelFunctions[FunctionNumber]->SetMemoryTypeOfArugment(ArgumentNumber, MemoryType, Issuccessful);
						if (Issuccessful)
						{
							if (!IsAllFunctionsUsable)
							{
								if (OpenCL_KernelFunctions[FunctionNumber]->IsThisListUsable)
								{
									TotalNumberOfFunctionsUsable = TotalNumberOfFunctionsUsable + 1;
									if (TotalNumberOfFunctionsUsable == TotalNumberOfFuctions)
									{
										IsAllFunctionsUsable = true;
									}
								}

							}
						}
						else
						{
							std::cout << "\n Error The Function cl_KernelFunctionArgumentOrderListStruct:SetMemoryTypeOfArugment failed in SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
						}
					}

				}
				else
				{
					std::cout << "\n Error Argument Number + 1' " << ArgumentNumber + 1 << "' Is greater than the TotalNumberOfArguments, ";
					if (IsAllFunctionsUsable)
					{
						std::cout << "Also all the functions are Fully Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
					}
					else
					{
						if (TotalNumberOfFunctionsUsable == 0)
						{
							std::cout << "But None Of functions Are Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
						}
						else
						{
							std::cout << "But only " << TotalNumberOfFunctionsUsable << "Number Of functions Are Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error SetMemoryTypeOfArugment() Failed in cl_KernelFunctionsStruct!";
			}
		}

		~cl_KernelFunctionsStruct()
		{
			std::cout << "\n Destructing cl_KernelFunctionsStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < TotalNumberOfFunctionsNameAndTotalArgumentSet; ++i)
				{
					if (OpenCL_KernelFunctions[i] != nullptr)
					{
						delete(OpenCL_KernelFunctions[i]);
					}					
				}
				free(OpenCL_KernelFunctions);
				OpenCL_KernelFunctions = nullptr;
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using, This is a Helper Struct for sending data to kernel(buffer), Also each of this is for 1 type of kernel only
	struct cl_KernelArgumentSendStruct
	{
	private:
		const cl_KernelFunctionArgumentOrderListStruct* ArugmentList;
		unsigned int** ArrayOfBufferToUse = nullptr;
		bool** ArrayOfOverWriteDataSetBoolean = nullptr;
		bool** ArrayOfIsArgumentAlreadySetBoolean = nullptr;
		bool** ArrayOfUsePreviouslyAllocatedMemoryBoolean = nullptr;
		Essenbp::UnknownDataAndSizeStruct** ArrayOfArgumentData = nullptr;

	public:
		bool IsConstructionSuccesful = false;
		bool DataPassedToKernel = false;

		cl_KernelArgumentSendStruct(cl_KernelFunctionArgumentOrderListStruct* ArgArugmentList, bool& Issuccessful) : ArugmentList(ArgArugmentList)
		{
			std::cout << "\n Constructing cl_KernelArgumentSendStruct";

			IsConstructionSuccesful = false;
			DataPassedToKernel = false;
			Issuccessful = false;

			if (!ArugmentList->IsConstructionSuccesful)
			{
				std::cout << "\n Error The ArugmentList Is Not Constructed, So Is Unusable In: cl_KernelArgumentSendStruct!\n";
			}
			else
			{
				if (!ArugmentList->IsThisListUsable)
				{
					std::cout << "\n Error ArugmentList Is Not Properly Set, So Is Unusable In: cl_KernelArgumentSendStruct!\n";
				}
				else
				{
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfBufferToUse, ArugmentList->TotalNumberOfArugments, sizeof(unsigned int*), Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(unsigned int*) << " Byes Of Memory for ArrayOfBufferToUse In: cl_KernelArgumentSendStruct!\n";
					}
					else
					{
						Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfOverWriteDataSetBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfOverWriteDataSetBoolean In: cl_KernelArgumentSendStruct!\n";
							free(ArrayOfBufferToUse);
						}
						else
						{
							Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfIsArgumentAlreadySetBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfIsArgumentAlreadySetBoolean In: cl_KernelArgumentSendStruct!\n";
								free(ArrayOfBufferToUse);
								free(ArrayOfOverWriteDataSetBoolean);
							}
							else
							{
								Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfUsePreviouslyAllocatedMemoryBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), Issuccessful);
								if (!Issuccessful)
								{
									std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfUsePreviouslyAllocatedMemoryBoolean In: cl_KernelArgumentSendStruct!\n";
									free(ArrayOfBufferToUse);
									free(ArrayOfOverWriteDataSetBoolean);
									free(ArrayOfIsArgumentAlreadySetBoolean);
								}
								else
								{
									Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfArgumentData, ArugmentList->TotalNumberOfArugments, sizeof(Essenbp::UnknownDataAndSizeStruct*), Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(Essenbp::UnknownDataAndSizeStruct*) << " Byes Of Memory for ArrayOfArgumentData In: cl_KernelArgumentSendStruct!\n";
										free(ArrayOfBufferToUse);
										free(ArrayOfOverWriteDataSetBoolean);
										free(ArrayOfIsArgumentAlreadySetBoolean);
										free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
									}
									else
									{
										for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
										{
											ArrayOfArgumentData[i] = new Essenbp::UnknownDataAndSizeStruct();
											ArrayOfBufferToUse[i] = new unsigned int(0);
											ArrayOfOverWriteDataSetBoolean[i] = new bool(false);
											ArrayOfIsArgumentAlreadySetBoolean[i] = new bool(false);
											ArrayOfUsePreviouslyAllocatedMemoryBoolean[i] = new bool(false);

											Issuccessful = (ArrayOfArgumentData[i] != nullptr) && (ArrayOfBufferToUse[i] != nullptr) && (ArrayOfIsArgumentAlreadySetBoolean[i] != nullptr) && (ArrayOfOverWriteDataSetBoolean[i] != nullptr) && (ArrayOfUsePreviouslyAllocatedMemoryBoolean[i] != nullptr);

											if (!Issuccessful)
											{
												if (ArrayOfArgumentData[i] != nullptr)
												{
													delete ArrayOfArgumentData[i];
												}
												else
												{
													std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(Essenbp::UnknownDataAndSizeStruct) << " Byes Of Memory for ArrayOfArgumentData[" << i << "] In: cl_KernelArgumentSendStruct!\n";
												}

												if (ArrayOfBufferToUse[i] != nullptr)
												{
													delete ArrayOfBufferToUse[i];
												}
												else
												{
													std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(unsigned int) << " Byes Of Memory for ArrayOfBufferToUse[" << i << "] In: cl_KernelArgumentSendStruct!\n";
												}

												if (ArrayOfOverWriteDataSetBoolean[i] != nullptr)
												{
													delete ArrayOfOverWriteDataSetBoolean[i];
												}
												else
												{
													std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfOverWriteDataSetBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
												}

												if (ArrayOfIsArgumentAlreadySetBoolean[i] != nullptr)
												{
													delete ArrayOfIsArgumentAlreadySetBoolean[i];
												}
												else
												{
													std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfIsArgumentAlreadySetBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
												}

												if (ArrayOfUsePreviouslyAllocatedMemoryBoolean[i] != nullptr)
												{
													delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[i];
												}
												else
												{
													std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfUsePreviouslyAllocatedMemoryBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
												}

												for (int j = 0; j < i; j++)
												{
													delete ArrayOfArgumentData[j];
													delete ArrayOfBufferToUse[j];
													delete ArrayOfOverWriteDataSetBoolean[j];
													delete ArrayOfIsArgumentAlreadySetBoolean[j];
													delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[j];
												}

												free(ArrayOfArgumentData);
												free(ArrayOfBufferToUse);
												free(ArrayOfOverWriteDataSetBoolean);
												free(ArrayOfIsArgumentAlreadySetBoolean);
												free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			if (!Issuccessful)
			{				
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		//NOTE: For LOCAL Data ArgData should be nullptr
		//NOTE: For PRIVATE Data ArgData should be not nullptr And ArgSizeOfData should match the size first inputed, If ArgData is nullptr and ArgSizeOfData is 0 then previously written memory will be reused
		//NOTE: For READ, WRITE and READ_AND_WRITE Data ArgData should NOT be nullptr and ArgSizeOfData should be great than 0 
		//NOTE: For READ, WRITE and READ_AND_WRITE Data If ArgData is nullptr and ArgSizeOfData is 0 then previously written memory will be reused
		//NOTE: 5th argument OverWriteMemory if set to true previous allocated memory location is reused NOTE: ArgSizeOfData should be less than or equal to previous memory! by default it is set to false
		//NOTE: 5th argument OverWriteMemory if set to true previous allocated memory location is reused NOTE: ArgSizeOfData should be less than or equal to previous memory! by default it is set to false
		void StoreDataForKernelArgument(unsigned int ArgumentNumber, unsigned int BufferTouse, void* ArgData, size_t ArgSizeOfData, bool& Issuccessful, bool OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling StoreDataForKernelArgument Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
			}
			else
			{
				if (ArgumentNumber >= ArugmentList->TotalNumberOfArugments)
				{
					std::cout << "\n Error ArgumentNumber + 1 Exceeds the Total Number Of Arugments Present! in StoreDataForKernelArgument In: PassDataStructToKernel!\n";
				}
				else
				{
					if (BufferTouse != *(ArrayOfBufferToUse[ArgumentNumber]))
					{
						*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]) = false;// Since we are changing buffer number
					}

					if (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_LOCALENUM)
					{
						if (OverWriteMemory_NOTForLOCAL_PRIVATE)
						{
							std::cout << "\n Error OverWriteMemory_NOTForLOCAL_PRIVATE is set to true, cl_Memory_Type::CL_LOCALENUM can not be written or overwritten StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
							std::cout << "NOTE: For PRIVATE and LOCAL Memory set OverWriteMemory_NOTForLOCAL_PRIVATE to false\n";
						}
						else
						{
							if (UsePreviouslyAllocatedMemoryOnBuffer)
							{
								std::cout << "\n Error UsePreviouslyAllocatedMemoryOnBuffer is set to true, cl_Memory_Type::CL_LOCALENUM can not be accesed from host StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								std::cout << "NOTE: For LOCAL Memory set UsePreviouslyAllocatedMemoryOnBuffer to false\n";
							}
							else
							{
								if (ArgData == nullptr)
								{
									ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(nullptr, ArgSizeOfData, Issuccessful, true);
									if (!Issuccessful)
									{
										std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
									}
								}
								else
								{
									std::cout << "\n Error Trying to Send Data to Local Memory, ArgData should be nullptr in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								}
							}
						}
					}
					else
					{
						if (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_PRIVATE)
						{
							if (OverWriteMemory_NOTForLOCAL_PRIVATE)
							{
								std::cout << "\n Error OverWriteMemory_NOTForLOCAL_PRIVATE is set to true, cl_Memory_Type::CL_PRIVATE is compatable with OverWriteMemory_NOTForLOCAL_PRIVATE StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								std::cout << "NOTE: For PRIVATE and LOCAL Memory set OverWriteMemory_NOTForLOCAL_PRIVATE to false\n";
							}
							else
							{
								if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))// After the First time(sencond and so on), the values are initialized to false at construction
								{
									if (ArgData == nullptr)
									{
										if (ArgSizeOfData == 0)
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												// Previous Data Will be reused
												Issuccessful = true;// setting it to succesful
											}
											else
											{
												std::cout << "\n Error Trying to Reuse previous Data but There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Trying to Reuse previous Data But ArgSizeOfData is Not Set to 0 in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
										}
									}
									else
									{
										if (ArgSizeOfData != ArrayOfArgumentData[ArgumentNumber]->GetDataSize())
										{
											std::cout << "\n Error Trying to Change the Size of cl_Memory_Type::CL_PRIVATE DataType in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											std::cout << "Size of Private Data type can only be set once, because Variable types do not changes their Data type sizes...\n";
										}
										else
										{
											if (ArgSizeOfData > 0)
											{
												ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, Issuccessful, false);
												//ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(nullptr, ArrayOfArgumentData[ArgumentNumber]->GetDataSize(), Issuccessful, true);// Same as above since ArgSizeOfData == ArrayOfArgumentData[ArgumentNumber]->GetDataSize()
												if (!Issuccessful)
												{
													std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
												}
											}
											else
											{
												if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
												{
													// Previous Data On the BUFFER Will be reused
													Issuccessful = true;// setting it to succesful
													//NOTE: This does not verify whether there is data on the buffer or not...
												}
												else
												{
													std::cout << "\n Error Size of Data for PRIVATE data can not be Less than 1 Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
													std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
												}
											}
										}
									}
								}
								else
								{
									if (ArgData != nullptr)
									{
										if (ArgSizeOfData > 0)
										{
											ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, Issuccessful, false);// For First time use
											if (!Issuccessful)
											{
												std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Size of Data for PRIVATE data can not be Less than 1 Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}
									else
									{
										if (ArgSizeOfData == 0)
										{
											if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
											{
												// Previous Data On the BUFFER Will be reused
												Issuccessful = true;// setting it to succesful
												//NOTE: This does not verify whether there is data on the buffer or not...
											}
											else
											{
												std::cout << "\n Error First time Setting Private Data but ArgData Is set to nullptr and size is 0 In: cl_KernelArgumentSendStruct!\n";
												std::cout << "NOTE: First Set data using valid pointer and size then you can reuse\n";
												std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
											}
										}
										else
										{
											std::cout << "\n Error First time Setting Private Data but ArgData Is set to nullptr In: cl_KernelArgumentSendStruct!\n";
											std::cout << "NOTE: First Set data using valid pointer and size then you can reuse\n";
											std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
										}
									}
								}
							}
						}
						else
						{
							if ((*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_READ_ONLY) || (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_WRITE_ONLY) || (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_READ_AND_WRITE))
							{
								if (ArgData != nullptr)
								{
									if (OverWriteMemory_NOTForLOCAL_PRIVATE)
									{
										if (!UsePreviouslyAllocatedMemoryOnBuffer)
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												if (ArgSizeOfData > ArrayOfArgumentData[ArgumentNumber]->GetDataSize())
												{
													std::cout << "\n Error Trying to OverWrite previous Data But ArgSizeOfData " << ArgSizeOfData << " Bytes Is Greater than Previous Data Size " << ArrayOfArgumentData[ArgumentNumber]->GetDataSize() << " Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
												}
												else
												{
													// Previous Data will be OverWritten
													ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, Issuccessful, false);
													if (!Issuccessful)
													{
														std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
													}
												}
											}
											else
											{
												std::cout << "\n Error Trying to OverWrite previous Data but There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Trying to OverWrite previous Data and UsePreviouslyAllocatedMemoryOnBuffer cannot be used at the same time, in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}
									else
									{
										ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, Issuccessful, false);
										if (!Issuccessful)
										{
											std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}
								}
								else
								{
									if (ArgSizeOfData == 0)
									{
										if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
										{
											// Previous Data On the BUFFER Will be reused
											Issuccessful = true;// setting it to succesful
											//NOTE: This does not verify whether there is data on the buffer or not...
										}
										else// While This Copys Data(Most definitely not modified) ON THE HOST to the buffer
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												// Previous Data on the HOST Will be reused
												Issuccessful = true;// setting it to succesful
											}
											else
											{
												std::cout << "\n Error Trying to Reuse previous Data on the HOST, But There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
									}
									else
									{
										std::cout << "\n Error Trying to Reuse previous Data But ArgSizeOfData is Not Set to 0 in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
									}
								}
							}
						}
					}
					//cl_Memory_Type::Uninitialized_cl_Memory or undefined memorytype is not possible here since list is declared fully initialized
				}

				if (!Issuccessful)// For the safe of readability
				{
					std::cout << "\n Error StoreDataForKernelArgument() Failed in cl_KernelArgumentSendStruct!";
				}
				else
				{
					DataPassedToKernel = false;// New data is stored so it means that data is not passed
					*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]) = true;
					if (OverWriteMemory_NOTForLOCAL_PRIVATE)
					{
						*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]) = true;
					}
					else
					{
						if (UsePreviouslyAllocatedMemoryOnBuffer)
						{
							*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[ArgumentNumber]) = true;
						}
						else
						{
							*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[ArgumentNumber]) = false;
						}
					}
				}
			}			
		}

		//This ArgumentNumberWhichIsNotReady -1 if true, else anywhere between 0 to (Total Numbers of Arguments - 1) which is the argument of the kernel function, to whom Data has not been sent
		void IsDataReadyForSend(bool& Issuccessful, cl_int& ArgumentNumberWhichIsNotReady)
		{
			Issuccessful = false;

			ArgumentNumberWhichIsNotReady = -1;// Means No Argument

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling IsDataReadyForSend Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
			}
			else
			{
				if (DataPassedToKernel)
				{
					//std::cout << "\n Error Same Data already passed to kernel in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
					//std::cout << "NOTE: Change any or all of argument data(or pass the same data) by using StoreDataForKernelArgument()\n";
					Issuccessful = true;// Since the Data is already passed and it was already succesful... this entire thing is succesful
				}
				else
				{
					for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
					{
						if (!(*(ArrayOfIsArgumentAlreadySetBoolean[i])))
						{
							ArgumentNumberWhichIsNotReady = i;
							break;
						}
					}
					if (ArgumentNumberWhichIsNotReady == -1)
					{
						Issuccessful = true;
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error StoreDataForKernelArgument() Failed in cl_KernelArgumentSendStruct!";
			}
		}

		//NOTE:Data will be passed to only one buffer for each arugment, the buffer number will be determined by SetBufferToUse() to manualy set, AllocateMemoryAndPassToKernel() which sets the specified buffer as the active buffer
		void PassAllDataToKernel(cl_KernelMultipleArgumentStruct* TheKernelMultiArgStruct, bool& Issuccessful, cl_int& ArgumentNumberWhichIsNotReady)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassAllDataToKernel Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
			}
			else
			{
				if (TheKernelMultiArgStruct == nullptr)
				{
					std::cout << "\n Error 'TheKernelMultiArgStruct Argument of type cl_KernelMultipleArgumentStruct Is a nullptr in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
				}
				else
				{
					if (!TheKernelMultiArgStruct->IsConstructionSuccesful)
					{
						std::cout << "\n Error 'TheKernelMultiArgStruct Argument of type cl_KernelMultipleArgumentStruct Is Not Constructed in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
					}
					else
					{
						IsDataReadyForSend(Issuccessful, ArgumentNumberWhichIsNotReady);// Data is Sent only if it is ready
						if (!Issuccessful)
						{
							if (ArgumentNumberWhichIsNotReady == -1)
							{
								std::cout << "\n Error A unexpected glitch may have occured... in PassDataStructToKernel  In: cl_KernelArgumentSendStruct!\n";
								std::cout << "IsDatReadForSend() Returned false but it also returned ArgumentNumberWhichIsNotReady = -1, Whish is IMPOSSIBLE check the code for the function for more details\n";
							}
							else
							{
								std::cout << "\n Error ArgumentList's argument Number " << ArgumentNumberWhichIsNotReady << " Is not Set! in PassDataStructToKernel In: cl_KernelArgumentSendStruct!\n";
							}
						}
						else
						{							
							for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
							{
								TheKernelMultiArgStruct->SetBufferToUse(i, *(ArrayOfBufferToUse[i]), Issuccessful);
								if (!Issuccessful)
								{
									std::cout << "\n Error cl_KernelSingleArgumentStruct::SetBufferToUse() Failed in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
									break;
								}

								if (*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[i]))// Uses previously allocated memory on buffer(if it exists)
								{
									TheKernelMultiArgStruct->AllocateMemoryAndPassToKernel(i, *(ArrayOfBufferToUse[i]), nullptr, 0, 0, false, true, Issuccessful);
								}
								else
								{
									TheKernelMultiArgStruct->AllocateMemoryAndPassToKernel(i, *(ArrayOfBufferToUse[i]), ArrayOfArgumentData[i]->GetData(), ArrayOfArgumentData[i]->GetDataSize(), (*(ArrayOfOverWriteDataSetBoolean[i]) ? 0 : ArrayOfArgumentData[i]->GetDataSize()), *(ArrayOfOverWriteDataSetBoolean[i]), false, Issuccessful);
								}	

								if (Issuccessful)// For the safe of readability
								{
									*(ArrayOfOverWriteDataSetBoolean[i]) = false;
									*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[i]) = false;
								}
								else
								{ 
									std::cout << "\n Error cl_KernelSingleArgumentStruct::AllocateMemoryAndPassToKernel() Failed in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
									break;
								}
							}							
						}						
					}
				}
			}
			
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error PassAllDataToKernel() Failed in cl_KernelArgumentSendStruct!";
			}
			else//On Success
			{
				DataPassedToKernel = true;
			}		
		}		

		~cl_KernelArgumentSendStruct()
		{
			std::cout << "\n Destructing cl_KernelArgumentSendStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
				{
					delete ArrayOfArgumentData[i];
					delete ArrayOfOverWriteDataSetBoolean[i];
					delete ArrayOfIsArgumentAlreadySetBoolean[i];
					delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[i];
				}
				free(ArrayOfArgumentData);
				free(ArrayOfOverWriteDataSetBoolean);
				free(ArrayOfIsArgumentAlreadySetBoolean);
				free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
			}
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: Call Constructor Before Using, This is a Helper for multi devices(single too)
	struct cl_MultiDevice_KernelArgumentSendStruct
	{
		const unsigned int NumberOfDevices;
		cl_KernelArgumentSendStruct** MultiArgumentSendStructList = nullptr;
		cl_KernelFunctionArgumentOrderListStruct* ArugmentList = nullptr;

		bool IsConstructionSuccesful = false;
		cl_MultiDevice_KernelArgumentSendStruct(unsigned int ArgNumberOfDevices, cl_KernelFunctionArgumentOrderListStruct* ArgArugmentList, bool& Issuccessful) : NumberOfDevices(ArgNumberOfDevices)
		{
			std::cout << "\n Constructing cl_MultiDevice_KernelArgumentSendStruct";
			MultiArgumentSendStructList = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;
			
			if (ArgArugmentList == nullptr)
			{
				std::cout << "\n Error ArgArugmentList is nullptr In: cl_MultiDevice_KernelArgumentSendStruct";
			}
			else
			{
				ArugmentList = ArgArugmentList;

				Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiArgumentSendStructList, NumberOfDevices, sizeof(cl_KernelArgumentSendStruct*), Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error Allocating " << NumberOfDevices * sizeof(cl_KernelArgumentSendStruct*) << " Byes Of Memory for MultiArgumentSendStructList in SetNumberOfDevices In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				}
				else
				{
					Issuccessful = false;
					for (int i = 0; i < NumberOfDevices; ++i)
					{
						MultiArgumentSendStructList[i] = new cl_KernelArgumentSendStruct(ArugmentList, Issuccessful);
						if (MultiArgumentSendStructList[i] == nullptr)
						{
							Issuccessful = false;
						}
						else
						{
							if (!Issuccessful)
							{
								delete MultiArgumentSendStructList[i];
							}
						}
						if (!Issuccessful)
						{
							std::cout << "\n Error Allocating " << NumberOfDevices * sizeof(cl_KernelArgumentSendStruct*) << " Byes Of Memory for MultiArgumentSendStructList[" << i << "] in SetNumberOfDevices In: cl_MultiDevice_KernelArgumentSendStruct!\n";
							for (int j = 0; j < i; ++j)
							{
								delete MultiArgumentSendStructList[j];
							}
							free(MultiArgumentSendStructList);
							MultiArgumentSendStructList = nullptr;
							break;
						}
					}
				}
			}

			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_MultiDevice_KernelArgumentSendStruct";
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void StoreDataForKernelArgument(unsigned int DeviceNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, void* ArgData, size_t ArgSizeOfData, bool& Issuccessful, bool OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false)
		{
			Issuccessful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling SetNumberOfDevices Without Constructing the struct In: cl_MultiDevice_KernelArgumentSendStruct!\n";
			}
			else
			{
				if (DeviceNumber < NumberOfDevices)
				{
					MultiArgumentSendStructList[DeviceNumber]->StoreDataForKernelArgument(ArgumentNumber, BufferNumber, ArgData, ArgSizeOfData, Issuccessful, OverWriteMemory_NOTForLOCAL_PRIVATE, UsePreviouslyAllocatedMemoryOnBuffer);
					if (!Issuccessful)
					{
						std::cout << "\n Error cl_KernelArgumentSendStruct::StoreDataForKernelArgument failed in StoreDataForKernelArgument In: cl_MultiDevice_KernelArgumentSendStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error DeviceNumber + 1 '" << DeviceNumber + 1 << "' is greater than NumberOfDevices in StoreDataForKernelArgument In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error StoreDataForKernelArgument() Failed in cl_MultiDevice_KernelArgumentSendStruct!";
			}
		}

		~cl_MultiDevice_KernelArgumentSendStruct()
		{
			std::cout << "\n Destructing cl_MultiDevice_KernelArgumentSendStruct!";
			if (IsConstructionSuccesful)
			{
				if (MultiArgumentSendStructList != nullptr)
				{
					for (int i = 0; i < NumberOfDevices; ++i)
					{
						delete MultiArgumentSendStructList[i];
					}
					free(MultiArgumentSendStructList);
					MultiArgumentSendStructList = nullptr;
				}
			}
			IsConstructionSuccesful = false;
		}
	};

	// IMPORTANT NOTE: This is the Only struct You will ever need, Because this Struct Does everything you want an OpenCL Wrapper to do
	struct cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct
	{
	private:
		const std::string ClSourceFilePath;
		cl_platform_id ChosenPlatform = NULL;
		cl_program SingleProgram = NULL;
		cl_context SingleContext = NULL;
		unsigned int NumberOfDevices = 0;
		cl_device_id* ChosenDevices = nullptr;
		cl_PerDeviceValuesStruct** PerDeviceValueStruct = nullptr;								// Initalized and Constructed in  InitializeOpenCLProgram()
		unsigned int TotalNumberOfKernelFunctions = 0;											// Don't try to manually change this unless you know how to do it properly
		cl_KernelFunctionArgumentOrderListStruct** OrderedKernelArgumentList = nullptr;			// This Contains All the Kernel Functions information
		cl_MultiKernelFunction_MultiDeviceStruct** MultiKernelFunction_MultiDevice = nullptr;	// Initialization And Construction functions will take care of it	

		//NOTE: This function Assumes the programkernelcode that was passed has correct syntax
		//NOTE: The ProgramKernelCode should not contain Comments, new line, and backspace in consequtive sequence. To Avoid That Run in this order -> Essenbp::RemoveCommentsFromCppSource(Original, Modified), Essenbp::ReplaceEveryOccuranceWithGivenString(Modified, "\n", " ") And Essenbp::RemoveConsecutiveDulplicateChar(Modified, ' ')
		void FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(std::string ProgramKernelCode, std::string FunctionName, cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, bool& Issuccessful)
		{
			Issuccessful = false;

			//PENDING
			if (*ArgOrderedKernelArgumentList != nullptr)
			{
				std::cout << "\n Error: *ArgOrderedKernelArgumentList is NOT nullptr, in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				size_t TotalNumberOfArgs = 0;
				size_t Position = 0;
				size_t TempPosition = 0;
				size_t VarPosition = 0;
				size_t PreVarPosition = 0;
				size_t CheckPosition = 0;
				size_t StartOfFunction = 0;
				size_t EndOfFunction = 0;

				if (FunctionName[0] != ' ')
				{
					FunctionName = " " + FunctionName;
				}
				if (FunctionName.back() != '(')
				{
					FunctionName = FunctionName + "(";
				}
				
				Issuccessful = true;
				Position = ProgramKernelCode.find(FunctionName);
				if (Position != std::string::npos)
				{
					Position = Position + (FunctionName.length() - 1);
					TempPosition = ProgramKernelCode.find(")", Position);
					StartOfFunction = ProgramKernelCode.find("{", TempPosition);

					//Find The End Of The Function('}')
					CheckPosition = StartOfFunction;
					EndOfFunction = StartOfFunction;
					PreVarPosition = 1;//	Total Number Of Opening Brackets
					VarPosition = 0;//		Total Number Of Closing Brackets
					while(true)
					{
						if (PreVarPosition > VarPosition)
						{
							EndOfFunction = ProgramKernelCode.find("}", EndOfFunction + 1);
							VarPosition = VarPosition + 1;

							while(true)
							{
								CheckPosition = ProgramKernelCode.find("{", CheckPosition + 1);
								if (Position != std::string::npos)
								{
									if (CheckPosition < EndOfFunction)
									{
										PreVarPosition = PreVarPosition + 1;
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}							
						}
						else
						{
							// End Of Function Found
							break;
						}
					}

					//First Get the total number of arguments and malloc
					VarPosition = Position;// Go to the Last Character FunctionName.back() which is '('
					PreVarPosition = Position;
					while (true)
					{
						if (VarPosition == TempPosition)
						{
							break;// Last argument already done so breaking
						}

						VarPosition = ProgramKernelCode.find(",", (VarPosition + 1));
						if ((VarPosition == std::string::npos) || (VarPosition > TempPosition))
						{
							PreVarPosition = Position;//  '('
							VarPosition = TempPosition;// ')'
						}
						TotalNumberOfArgs = TotalNumberOfArgs + 1;
					}

					

					if (Issuccessful)
					{
						//Finds all argument and its type inside the function
						VarPosition = Position;// Go to the Last Character FunctionName.back() which is '('
						PreVarPosition = Position;
						while (true)
						{
							if (VarPosition == TempPosition)
							{
								break;// Last argument already done so breaking
							}

							VarPosition = ProgramKernelCode.find(",", (VarPosition + 1));
							if ((VarPosition == std::string::npos) || (VarPosition > TempPosition))
							{
								PreVarPosition = Position;//  '('
								VarPosition = TempPosition;// ')'
							}

							//PENDING
							Essenbp::FindStartOfSubStringInString(ProgramKernelCode, "global", PreVarPosition, VarPosition, CheckPosition, Issuccessful);
							if (Issuccessful)
							{
								//ReadOnly,WriteOnly and ReadAndWrite global any of three
								//NOTE: ProgramKernelCode SHOULD NOT contain Comments, new line, and backspace in consequtive sequence. To Avoid That Run in this order -> Essenbp::RemoveCommentsFromCppSource(Original, Modified), Essenbp::ReplaceEveryOccuranceWithGivenString(Modified, "\n", " ") And Essenbp::RemoveConsecutiveDulplicateChar(Modified, ' ')
								//Explaination: let this be a parameter (global int* StorageOne, global int* StorageTwo)
								//The Below for Loop looks for the Full Name of the Argument

								bool IsReadOnly = false;
								bool IsWriteOnly = false;
								size_t i = VarPosition;// Var Position could be ',' or ')'(Last Argument/Only One Argument) Depending on the loop
								if (ProgramKernelCode[(VarPosition - 1)] == ' ')
								{
									i = i - 2;// Goes to the Last Character of the Arugment Name(Assuming that there is only 1 BackSpace between ","/")" and the last character of the Arugment Name)
								}
								else
								{
									i = i - 1;// If not then This is the Last Character Of the Arugment Name
								}
								for (i = i; i > PreVarPosition; --i)
								{
									if (ProgramKernelCode[i] == ' ')// From Last Character to First character we Loop, If a BackSpace is Found then it means the First Charcter is (i + 1)
									{
										i = i + 1;
										break;
									}
								}

								//PENDING check the variable type
								std::string ArgumentName = " " + ProgramKernelCode.substr(i, ((VarPosition + 1) - i));// " ArgumentName" Find this in the loop
								CheckPosition = StartOfFunction;

								//Find Every Occurance of the ArgumentName
								while (true)
								{
									//Check for Reads First Then Writes Next
									CheckPosition = ProgramKernelCode.find(ArgumentName, (CheckPosition + 1));
									if (VarPosition == std::string::npos)
									{
										break;
									}
									else
									{
										if (CheckPosition < EndOfFunction)
										{
											//Check if it is read or write

											//Check On Left
											if (ProgramKernelCode[(CheckPosition - 1)] == ' ')
											{
												i = 2;
											}
											else
											{
												i = 1;
											}
											switch (ProgramKernelCode[(CheckPosition - i)])
											{
												//case ' ':// Impossible for this to happen since the above is alreayd done
												//{
												//	break;
												//}
											case ','://Could mean this variable is used as parameter for a user/inbuilt function
											{
												break;//PENDING
											}
											case '('://Could mean this variable is used as parameter for a user/inbuilt function
											{
												break;//PENDING
											}
											//case ')'://Could mean this variable is used as parameter for a user/inbuilt function
											//{
											//	break;//Not here
											//}
											case '=':// Check if it is Assignment(=) or Comparison(==) operator 
											{
												if (ProgramKernelCode[((CheckPosition - 1) - i)] == '=')
												{
													//Comparison(==) operator confirmed
													IsReadOnly = true;
												}
												else
												{
													//Assignment(=) operator confirmed
													IsWriteOnly = true;
												}
												break;
											}
											//case '.':// It means this is not the variable but instead a member of some other variable type Example: "OtherType.ArgumentName"
											//{
											//	break;
											//}
											//case ';':
											//{
											//	break;
											//}
											//case '{':
											//{
											//	break;
											//}
											//case '}':
											//{
											//	break;
											//}
											default: // code to be executed if n doesn't match any cases
											{

												break;
											}
											}

											//Check On Right
											if (ProgramKernelCode[(CheckPosition + 1)] == ' ')
											{
												i = 2;
											}
											else
											{
												i = 1;
											}
											switch (ProgramKernelCode[(CheckPosition + i)])
											{
												//case ' ':// Impossible for this to happen since the above is alreayd done
												//{
												//	break;
												//}
											case ','://Could mean this variable is used as parameter for a user/inbuilt function
											{
												break;//PENDING
											}
											case '('://Could mean this variable is used as parameter for a user/inbuilt function
											{
												break;//PENDING
											}
											//case ')'://Could mean this variable is used as parameter for a user/inbuilt function
											//{
											//	break;//Not here
											//}
											case '=':// Check if it is Assignment(=) or Comparison(==) operator 
											{
												if (ProgramKernelCode[((CheckPosition + 1) + i)] == '=')
												{
													//Comparison(==) operator confirmed
													IsReadOnly = true;
												}
												else
												{
													//Assignment(=) operator confirmed
													IsWriteOnly = true;
												}
												break;
											}
											//case '.':// It means this is not the variable but instead a member of some other variable type Example: "OtherType.ArgumentName"
											//{
											//	break;
											//}
											//case ';':
											//{
											//	break;
											//}
											//case '{':
											//{
											//	break;
											//}
											//case '}':
											//{
											//	break;
											//}
											default: // code to be executed if n doesn't match any cases
											{
												break;
											}
											}

											if (IsReadOnly && IsWriteOnly)
											{
												break;
											}
										}
										else
										{
											break;
										}
									}
								}

								if ((!IsReadOnly) && (!IsWriteOnly))
								{
									IsReadOnly = true;//When The memory type is not being written on or being read, it is set to read only by default
								}

							}
							else
							{
								Essenbp::FindStartOfSubStringInString(ProgramKernelCode, "local", PreVarPosition, VarPosition, CheckPosition, Issuccessful);
								if (Issuccessful)
								{
									//Local
								}
								else
								{
									Essenbp::FindStartOfSubStringInString(ProgramKernelCode, "constant", PreVarPosition, VarPosition, CheckPosition, Issuccessful);
									if (Issuccessful)
									{
										//ReadOnly global
									}
									else
									{
										// Private
									}
								}
							}

							PreVarPosition = VarPosition;
						}
					}
					//if (TempPosition != std::string::npos)
					//{
					//	
					//}
					//else
					//{
					//	//Unlikely to happen
					//	Issuccessful = false;
					//	std::cout << "\n Error ')' Was not Found in the given Program Code in FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
					//}
				}
				else
				{
					std::cout << "\n Error "<< FunctionName <<" Was not Found in the given Program Code in FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

	public:
		bool IsConstructionSuccesful = false;// Same as before, Manual changes = memory leaks, Automatic(constructor) Only changes will Obliterate the chances of possible memory leaks

		//Initialization

		void ReleaseAndFreeClData(bool& Issuccessful)
		{
			Issuccessful = true;

			cl_int ClErrorResult;
			if (SingleProgram != NULL)
			{
				ClErrorResult = clReleaseProgram(SingleProgram);
				if (ClErrorResult != CL_SUCCESS)
				{
					Issuccessful = false;
					std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_program " << "in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				else
				{
					SingleProgram = NULL;
				}
			}
			if (SingleContext != NULL)
			{
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					Issuccessful = false;
					std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				else
				{
					SingleContext = NULL;
				}
			}
			if (PerDeviceValueStruct != nullptr)
			{
				for (int i = 0; i < NumberOfDevices; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				PerDeviceValueStruct = nullptr;
			}
			if (ChosenDevices != nullptr)
			{
				free(ChosenDevices);
				ChosenDevices = nullptr;
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error ReleaseAndFreeClData() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		void InitializeOpenCLProgram(bool& Issuccessful, cl_uint PlatformToUse)//, cl_uint PlatformToUse = 1)
		{
			Issuccessful = false;

			cl_uint NumOfPlatforms;						//the NO. of platforms
			//ChosenPlatform = nullptr;					////the chosen platform Will only be choosing one platform which will be the first one
			cl_uint	NumberOfGPUs = 0;					//cl_uint	NumberOfGPUorCPU = 0;// We will Only be using GPU here

			bool ReturnResult = true;
			cl_int ClErrorResult = clGetPlatformIDs(0, NULL, &NumOfPlatforms);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting platforms in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			ClErrorResult = clGetPlatformIDs(PlatformToUse, &ChosenPlatform, NULL);// for the first platform //Single Line code
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Platform Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &NumberOfGPUs);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting GPUs in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}
			std::cout << "\n\nTotal Number Of GPUs Available: " << NumberOfGPUs << "\n";

			if (NumberOfGPUs < 1)
			{
				std::cout << "\n : Error NO GPUs Available in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			ChosenDevices = (cl_device_id*)malloc(NumberOfGPUs * sizeof(cl_device_id));
			if (ChosenDevices == nullptr)
			{
				std::cout << "\n Error Allocating : " << NumberOfGPUs * sizeof(cl_device_id) << " Byes Of Memory for ChosenDevices in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, NumberOfGPUs, ChosenDevices, NULL);// Same line for code for Multi-GPU
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : GPUs Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			SingleContext = clCreateContext(NULL, NumberOfGPUs, ChosenDevices, NULL, NULL, &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				free(ChosenDevices);
				std::cout << "\n ClError Code " << ClErrorResult << " : Context Not Created in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&PerDeviceValueStruct, NumberOfGPUs, sizeof(cl_PerDeviceValuesStruct*), Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error Allocating " << (NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*)) << " Byes Of Memory for PerDeviceValueStruct In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				free(ChosenDevices);
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
			else
			{
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					//Create a command queue for every device
					PerDeviceValueStruct[i] = new cl_PerDeviceValuesStruct( ChosenDevices[i], &SingleContext, ClErrorResult, Issuccessful );
					if (PerDeviceValueStruct[i] == nullptr)
					{
						Issuccessful = false;
					}
					else
					{
						if (!Issuccessful)
						{
							delete PerDeviceValueStruct[i];
						}
					}
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating " << sizeof(cl_PerDeviceValuesStruct) << " Byes Of Memory for PerDeviceValueStruct[" << i << "] In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							delete PerDeviceValueStruct[j];
						}
						ReleaseAndFreeClData(Issuccessful);// Do nothing if Failed
						Issuccessful = false;
						break;
					}					
				}

				if (Issuccessful)
				{
					//const char* ClSourceFilePath = "D:\\C++ Test Projects\\TestOpenCL4\\PunalManalanOpenCLKernelFunctions.cl";
					std::string ClSourceFileInString;
					Essenbp::GetTextFileContent(ClSourceFilePath, ClSourceFileInString, Issuccessful);
					const char* ClSourceFileInChar = ClSourceFileInString.c_str();
					size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
					SingleProgram = clCreateProgramWithSource(SingleContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, &ClErrorResult);

					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n ClError Code " << ClErrorResult << " : Cl Program Not Created with Source!\n";
						for (int i = 0; i < NumberOfGPUs; i++)
						{
							delete(PerDeviceValueStruct[i]);
						}
						ReleaseAndFreeClData(Issuccessful);// Do nothing if Failed
						Issuccessful = false;
					}
					else
					{
						ClErrorResult = clBuildProgram(SingleProgram, 0, NULL, NULL, NULL, NULL);
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Cl Program Not Build in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							for (int i = 0; i < NumberOfGPUs; i++)
							{
								delete(PerDeviceValueStruct[i]);
							}
							ReleaseAndFreeClData(Issuccessful);// Do nothing if Failed
							Issuccessful = false;
						}
					}
				}
				else
				{
					std::cout << "\n Error OpenCL Program File Was not opened,\n" << ClSourceFilePath << "\n in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}		
			

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InitializeOpenCLProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
			else
			{
				NumberOfDevices = NumberOfGPUs;
			}
		}
		
		void FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(std::string ProgramKernelCode, std::string& ModifiedKernelCode, cl_KernelFunctionArgumentOrderListStruct*** ArgOrderedKernelArgumentList, unsigned int& ArgTotalNumberOfKernelFunctions, bool& Issuccessful)
		{
			ArgTotalNumberOfKernelFunctions = 0;

			Issuccessful = false;

			if (*ArgOrderedKernelArgumentList != nullptr)
			{
				std::cout << "\n Error The value of the variable 'ArgOrderedKernelArgumentList' Is not nullptr, It should be empty, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				//PENDING
				if (!Issuccessful)
				{
					std::cout << "\n Error: Functions_List of Type cl_KernelFunctionsStruct Failed Construction, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				else
				{
					if(*ArgOrderedKernelArgumentList != nullptr)
					{
						std::cout << "\n Error: *ArgOrderedKernelArgumentList is NOT nullptr, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else
					{
						//First We Delete unwanted Comments
						ModifiedKernelCode = "";
						Essenbp::RemoveCommentsFromCppSource(ProgramKernelCode, ModifiedKernelCode);//First, Comments are removed	
						Essenbp::ReplaceEveryOccuranceWithGivenString(ModifiedKernelCode, "\n", " ");// new line replaced with back space
						Essenbp::RemoveConsecutiveDulplicateChar(ModifiedKernelCode, ' ');// Consecutive Duplicates removed
									
						Essenbp::UnknownDataAndSizeStruct Storage;//Stoare the pointer to orderdlist here					
						size_t Position = 0;
						size_t TempPosition = 0;
												
						//Then attributes are removed in the while loop below
						while (true)
						{
							Position = ModifiedKernelCode.find("__attribute__", Position);
							if (Position != std::string::npos)
							{
								TempPosition = ModifiedKernelCode.find(")", Position);
								if (TempPosition != std::string::npos)
								{
									TempPosition = ModifiedKernelCode.find(")", (TempPosition + 1));
									if (TempPosition != std::string::npos)
									{
										TempPosition = ModifiedKernelCode.find(")", (TempPosition + 1));
										if (TempPosition != std::string::npos)
										{
											ModifiedKernelCode.erase(Position, TempPosition);
										}
										else
										{
											Issuccessful = false;// Wrong systax results in this...
										}
									}
									else
									{
										Issuccessful = false;
									}
								}
								else
								{
									Issuccessful = false;
								}
							}
							else
							{
								cl_int ClErrorResult = 0;
								cl_context CheckingContext = NULL;
								const char* ClSourceFileInChar = ProgramKernelCode.c_str();//Not Using Modified
								size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
								SingleProgram = clCreateProgramWithSource(CheckingContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, &ClErrorResult);

								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "\n ClError Code " << ClErrorResult << " : Cl Program Not Created with Source!\n";
									Issuccessful = false;
								}
								else
								{
									clReleaseContext(CheckingContext);
									Issuccessful = true;
								}								
								break;
							}
						}

						if (Issuccessful)
						{
							cl_KernelFunctionArgumentOrderListStruct* OrderedListCopyPointer = nullptr;
							Position = 0;
							TempPosition = 0;

							while (true)
							{
								Issuccessful = false;
								OrderedListCopyPointer = nullptr;

								Position = ModifiedKernelCode.find("kernel ", Position);// look for kernel keyword
								if (Position != std::string::npos)
								{
									// Checking to see If the "Kernel" word is a part of a name like this for example "AsdfKernel "//This is a example of a function name,
									if (Position != 0)
									{
										Issuccessful = true;
									}
									else
									{
										if (ModifiedKernelCode[(Position - 1)] == ' ')
										{
											Issuccessful = true;
										}
										else
										{
											// Not a keyword but a function name,
											Position = Position + 7;
										}
									}

									if (Issuccessful)
									{
										Position = Position + 6;
										Position = ModifiedKernelCode.find(" void ", Position);// look for void keyword
										if (Position != std::string::npos)
										{
											Issuccessful = false;
											std::cout << "\n Error: unable to find \" void \" keyword. The syntax of the Program Kernel Code is Incorrect... in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
											break;
										}
										else
										{
											Position = Position + 6;//First Character Of Function Name

											TempPosition = 0;
											TempPosition = ModifiedKernelCode.find("(", Position);

											if (TempPosition == Position)
											{
												Issuccessful = false;
												std::cout << "\n Error: The name of the function is Missing. The syntax of the Program Kernel Code is Incorrect... in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
											}
											else
											{
												FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ModifiedKernelCode, ModifiedKernelCode.substr(Position, ((TempPosition + 1) - Position)), &OrderedListCopyPointer, Issuccessful);
												if (!Issuccessful)
												{
													std::cout << "\n Error: FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
													//PENDING
													//DELETE Data in STORAGE one by one
												}
												else
												{
													Storage.CopyAndStoreData(OrderedListCopyPointer, sizeof(cl_KernelFunctionArgumentOrderListStruct*), Issuccessful, false, true);
													if (!Issuccessful)
													{
														std::cout << "\n Error: FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
														//PENDING
														//DELETE Data in STORAGE one by one
													}
													else
													{
														ArgTotalNumberOfKernelFunctions = ArgTotalNumberOfKernelFunctions + 1;
													}
												}
												//PENDING
												//Put that function variable finding funciton here
											}
										}
									}
								}
								else
								{
									if (Position == 0)
									{
										std::cout << "\n Error: The Input Program Kernel Code does not havey any Kernels...in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
										Issuccessful = false;
									}
									else
									{
										Issuccessful = true;
									}
									break;
								}
							}
						}
						else
						{
							std::cout << "\n Error: The syntax of the Program Kernel Code is Incorrect... in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}

						//FINAL CONTINUE
						if (Issuccessful)
						{
							Essenbp::Malloc_PointerToArrayOfPointers((void***)ArgOrderedKernelArgumentList, ArgTotalNumberOfKernelFunctions, sizeof(cl_KernelFunctionArgumentOrderListStruct*), Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error Allocating " << (ArgTotalNumberOfKernelFunctions * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for ArgTotalNumberOfKernelFunctions in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
							else
							{
								//CONTINUE
								for (int i = 0; i < ArgTotalNumberOfKernelFunctions; ++i)
								{
									FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ModifiedKernelCode, )
									ArgOrderedKernelArgumentList[i] = new cl_KernelFunctionArgumentOrderListStruct(, Issuccessful);
								}
							}							
						}
					}										
				}

				if (Issuccessful)
				{
					Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error: SetTheNameAndNumberOfArgumentsForNextKernelFunction Failed in FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}				
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}
		
		void cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(std::string ClSourceFilePath, cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, unsigned int ArgTotalNumberOfKernelFunctions, bool& Issuccessful)
		{
			Issuccessful = false;

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiKernelFunction_MultiDevice, TotalNumberOfKernelFunctions, sizeof(cl_MultiKernelFunction_MultiDeviceStruct*), Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_MultiKernelFunction_MultiDeviceStruct*)) << " Byes Of Memory for MultiKernelFunction_MultiDevice In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					MultiKernelFunction_MultiDevice[i] = new cl_MultiKernelFunction_MultiDeviceStruct(NumberOfDevices, OrderedKernelArgumentList[i], &SingleContext, PerDeviceValueStruct[i], &SingleProgram, Issuccessful);
					
					if (MultiKernelFunction_MultiDevice[i] == nullptr)
					{
						Issuccessful = false;
					}
					else
					{
						if (!Issuccessful)
						{
							delete MultiKernelFunction_MultiDevice[i];
						}
					}
					if (!Issuccessful)
					{
						std::cout << "\n Error Allocating " << (sizeof(cl_MultiKernelFunction_MultiDeviceStruct)) << " Byes Of Memory for MultiKernelFunction_MultiDevice[i] In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						IsConstructionSuccesful = false;

						for (int j = 0; j < i; ++j)
						{
							delete MultiKernelFunction_MultiDevice[j];
						}
						free(MultiKernelFunction_MultiDevice);
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		// Manual
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(std::string ClSourceFilePath, cl_KernelFunctionsStruct* KernelFunctionsStruct, unsigned int ArgTotalNumberOfKernelFunctions, cl_PlatformVendorStruct* PlatformToUse, bool& Issuccessful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Manual Constructor";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = ArgTotalNumberOfKernelFunctions;
			OrderedKernelArgumentList = nullptr;
			MultiKernelFunction_MultiDevice = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if ((KernelFunctionsStruct == nullptr) || (PlatformToUse == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
			else
			{
				cl_uint PlatformNumberToUse = 0;
				PlatformToUse->GetChosenPlatform(PlatformNumberToUse, Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
				else
				{
					// Single Program Is First Initialized
					InitializeOpenCLProgram(Issuccessful, PlatformNumberToUse);

					Issuccessful = Issuccessful && KernelFunctionsStruct->IsConstructionSuccesful && KernelFunctionsStruct->IsAllFunctionsUsable;
					if (Issuccessful)
					{
						Essenbp::Malloc_PointerToArrayOfPointers((void***)&OrderedKernelArgumentList, TotalNumberOfKernelFunctions, sizeof(cl_KernelFunctionArgumentOrderListStruct*), Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for OrderedKernelArgumentList In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
						else
						{
							for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
							{
								OrderedKernelArgumentList[i] = new cl_KernelFunctionArgumentOrderListStruct(KernelFunctionsStruct->OpenCL_KernelFunctions[i], Issuccessful);
								if (OrderedKernelArgumentList[i] == nullptr)
								{
									Issuccessful = false;
								}
								else
								{
									if (!Issuccessful)
									{
										delete OrderedKernelArgumentList[i];
									}
								}
								if (!Issuccessful)
								{
									std::cout << "\n Error Allocating " << sizeof(cl_KernelFunctionArgumentOrderListStruct) << " Byes Of Memory for OrderedKernelArgumentList[" << i << "] In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									IsConstructionSuccesful = false;

									for (int j = 0; j < i; ++j)
									{
										delete OrderedKernelArgumentList[j];
									}
									free(OrderedKernelArgumentList);
								}
							}
							if (Issuccessful)
							{
								cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, Issuccessful);
								if (!Issuccessful)
								{
									std::cout << "\n Error cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
									{
										delete OrderedKernelArgumentList[i];
									}
								}
							}
						}
					}
				}
			}
			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct Manual!";
				ReleaseAndFreeClData(Issuccessful);// Do nothing if Failed
				Issuccessful = false;
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		// Automatic
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath, cl_PlatformVendorStruct* PlatformToUse, bool& Issuccessful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Automatic Constructor";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = 0;
			OrderedKernelArgumentList = nullptr;
			MultiKernelFunction_MultiDevice = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			if (PlatformToUse == nullptr)
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
			else
			{
				cl_uint PlatformNumberToUse = 0;
				PlatformToUse->GetChosenPlatform(PlatformNumberToUse, Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
				else
				{
					// Single Program Is First Initialized	
					InitializeOpenCLProgram(Issuccessful, PlatformNumberToUse);

					if (Issuccessful)
					{
						std::string ClSourceFileInString;
						Essenbp::GetTextFileContent(ClSourceFilePath, ClSourceFileInString, Issuccessful);
						if (Issuccessful)
						{
							Issuccessful = false;
							FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
							else
							{
								for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
								{
									if (OrderedKernelArgumentList[i]->IsConstructionSuccesful)
									{
										if (OrderedKernelArgumentList[i]->IsThisListUsable)
										{
											FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ClSourceFileInString, OrderedKernelArgumentList[i]->KernelFunctionName, *(OrderedKernelArgumentList[i]), Issuccessful);											
											if (!Issuccessful)
											{
												std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
												break;
											}
										}
										else
										{											
											std::cout << "\n Error FOrderedKernelArgumentList["<< i <<"]->IsThisListUsable Is Set to false In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
											Issuccessful = false;
											break;
										}
									}
									else
									{
										std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
										Issuccessful = false;
										break;										
									}
								}

								if (Issuccessful)
								{
									cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";																				
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error OpenCL Program File Was not opened,\n" << ClSourceFilePath << "\n In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";							
						}						
					}
					else
					{
						std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";						
					}
				}
			}			
			
			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct Automatic!";
				if (OrderedKernelArgumentList != nullptr)
				{
					for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
					{
						delete OrderedKernelArgumentList[i];
					}
				}
				ReleaseAndFreeClData(Issuccessful);
				Issuccessful = false;// Do nothing if Failed
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		// Single Line Automatic, First Platform Is chosen by default
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath, bool& Issuccessful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Single Line Automatic Automatic Constructor(NOTE: First Platform Is chosen by default)";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = 0;
			OrderedKernelArgumentList = nullptr;
			MultiKernelFunction_MultiDevice = nullptr;

			IsConstructionSuccesful = false;
			Issuccessful = false;

			OCLW_P::cl_PlatformVendorStruct PlatformToUse(Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error Construction of 'PlatformToUse' Variable of type 'OCLW_P::cl_PlatformVendorStruct' Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";				
			}

			int TotalNumberOfPlatforms;
			PlatformToUse.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
			if (!Issuccessful)
			{
				std::cout << "\n Error GetTotalNumberOfPlatformVendors() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
			else
			{
				PlatformToUse.PrintAllAvailablePlatformVendorNames(Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error PrintAllAvailablePlatformVendorNames() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
				else
				{
					PlatformToUse.SetChosenPlatform(1, Issuccessful);
					if (!Issuccessful)
					{
						std::cout << "\n Error SetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
					}					
				}				
			}
			if (Issuccessful)
			{
				cl_uint PlatformNumberToUse = 0;
				PlatformToUse.GetChosenPlatform(PlatformNumberToUse, Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}

				// Single Program Is First Initialized	
				InitializeOpenCLProgram(Issuccessful, PlatformNumberToUse);

				PlatformToUse.GetChosenPlatform(PlatformNumberToUse, Issuccessful);
				if (!Issuccessful)
				{
					std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
				else
				{
					// Single Program Is First Initialized	
					InitializeOpenCLProgram(Issuccessful, PlatformNumberToUse);

					if (Issuccessful)
					{
						std::string ClSourceFileInString;
						Essenbp::GetTextFileContent(ClSourceFilePath, ClSourceFileInString, Issuccessful);
						if (Issuccessful)
						{
							Issuccessful = false;
							FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
							else
							{
								for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
								{
									if (OrderedKernelArgumentList[i]->IsConstructionSuccesful)
									{
										if (OrderedKernelArgumentList[i]->IsThisListUsable)
										{
											FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ClSourceFileInString, OrderedKernelArgumentList[i]->KernelFunctionName, *(OrderedKernelArgumentList[i]), Issuccessful);
											if (!Issuccessful)
											{
												std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
												break;
											}
										}
										else
										{
											std::cout << "\n Error FOrderedKernelArgumentList[" << i << "]->IsThisListUsable Is Set to false In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
											Issuccessful = false;
											break;
										}
									}
									else
									{
										std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
										Issuccessful = false;
										break;
									}
								}

								if (Issuccessful)
								{
									cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error OpenCL Program File Was not opened,\n" << ClSourceFilePath << "\n In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}
			}

			if (!Issuccessful)
			{
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct Automatic Single Line!";
				if (OrderedKernelArgumentList != nullptr)
				{
					for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
					{
						delete OrderedKernelArgumentList[i];
					}
				}
				ReleaseAndFreeClData(Issuccessful);
				Issuccessful = false;// Do nothing if Failed
			}
			else
			{
				IsConstructionSuccesful = true;
			}
		}

		void PassDataStructToKernel(unsigned int KernelToRunNumber, unsigned int DevicesToSendData_From, unsigned int DevicesToSendData_To, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassDataStructToKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				if (MultiDeviceSendStructList == nullptr)
				{
					std::cout << "\n Error :Passed Argument MultiDeviceSendStructList Is nullptr in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				else
				{
					if (!MultiDeviceSendStructList->IsConstructionSuccesful)
					{
						std::cout << "\n Error :Passed Argument MultiDeviceSendStructList Is Not Consturcted in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else
					{
						if (KernelToRunNumber < TotalNumberOfKernelFunctions)
						{
							cl_int PossiblePendingArgument = -1;

							for (int i = DevicesToSendData_From; i <= DevicesToSendData_To; ++i)
							{
								MultiDeviceSendStructList->MultiArgumentSendStructList[i]->IsDataReadyForSend(Issuccessful, PossiblePendingArgument);
								if (!Issuccessful)
								{
									if (PossiblePendingArgument == -1)
									{
										std::cout << "\n Error Unconstructed ArgumentList variable of type cl_KernelArgumentSendStruct& Passed in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									}
									else
									{
										std::cout << "\n Error ArgumentList's argument Number " << PossiblePendingArgument << " Is not Set! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									}
									break;
								}
							}

							if (Issuccessful)
							{
								Issuccessful = false;
								if (DevicesToSendData_From > DevicesToSendData_To)
								{
									std::cout << "\n Error DevicesToSendData_From Is greater than DevicesToSendData_To! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
								}
								else
								{
									if (DevicesToSendData_To < NumberOfDevices)
									{
										for (int i = DevicesToSendData_From; i <= DevicesToSendData_To; ++i)
										{
											MultiDeviceSendStructList->MultiArgumentSendStructList[i]->PassAllDataToKernel(MultiKernelFunction_MultiDevice[KernelToRunNumber]->MultiDeviceKernelArgumentsArray[i], Issuccessful, PossiblePendingArgument);
											if (!Issuccessful)
											{
												std::cout << "\n Error cl_KernelArgumentSendStruct::PassAllDataToKernel failed in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
												break;
											}
										}
									}
									else
									{
										std::cout << "\n Error DevicesToSendData_To Exceeds the Number Of Devices Present! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error KernelToRunNumber Exceeds the Total Number Of Kernel Functions Present! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error PassDataStructToKernel() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		void PassDataStructToKernel(std::string NameOfTheKernelToRun, unsigned int DevicesToSendData_From, unsigned int DevicesToSendData_To, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassDataStructToKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernelToRun, Issuccessful);
			if (Issuccessful)
			{
				PassDataStructToKernel(KernelNumber, DevicesToSendData_From, DevicesToSendData_To, MultiDeviceSendStructList, Issuccessful);
				return;
			}
			else
			{
				//std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernelToRun << " Is Not Found In the Provided Program!\n";
				//std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}
		}

		void RunKernelFunction(unsigned int KernelToRunNumber, unsigned int DevicesToRunKernel_From, unsigned int DevicesToRunKernel_To, cl_MultiDevice_NDRangeStruct* MultiDeviceNDRange, bool& Issuccessful, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList = nullptr)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RunKernelFunction Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				if (KernelToRunNumber < TotalNumberOfKernelFunctions)
				{
					if (DevicesToRunKernel_From > DevicesToRunKernel_To)
					{
						std::cout << "\n Error DevicesToRunKernel_From Is greater than DevicesToRunKernel_To! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else
					{
						if (DevicesToRunKernel_To < NumberOfDevices)
						{
							Issuccessful = true;// Yes this is correct
							if (MultiDeviceSendStructList != nullptr)
							{
								PassDataStructToKernel(KernelToRunNumber, DevicesToRunKernel_From, DevicesToRunKernel_To, MultiDeviceSendStructList, Issuccessful);
								if (!Issuccessful)// For the safe of readability
								{
									std::cout << "\n Error PassDataStructToKernel() failed in RunKernelFunction in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
								}
							}

							if (Issuccessful)
							{
								cl_NDRangeStruct* PointerToNDRangeStruct = nullptr;
								for (int i = DevicesToRunKernel_From; i <= DevicesToRunKernel_To; ++i)
								{
									MultiDeviceNDRange->GetNDRangeOfDevice(i, &PointerToNDRangeStruct, Issuccessful);
									if (!Issuccessful)
									{
										std::cout << "\n Error MultiDeviceNDRange.GetNDRangeOfDevice() for Device '" << i << "' Failed in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									}
									else
									{
										MultiKernelFunction_MultiDevice[KernelToRunNumber]->RunKernel(i, PointerToNDRangeStruct, Issuccessful);
										if (!Issuccessful)
										{
											std::cout << "\n Error MultiKernelFunction_MultiDevice[KernelToRunNumber]->RunKernel(" << i << ",arg,arg) in RunKernelFunction in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
										}
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error DevicesToRunKernel_To + 1 Exceeds the Number Of Devices Present! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
				}
				else
				{
					std::cout << "\n Error KernelToRunNumber Exceeds the Total Number Of Kernel Functions Present! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RunKernelFunction() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}
		void RunKernelFunction(std::string NameOfTheKernelToRun, unsigned int DevicesToRunKernel_From, unsigned int DevicesToRunKernel_To, cl_MultiDevice_NDRangeStruct* MultiDeviceNDRange, bool& Issuccessful, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList = nullptr)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RunKernelFunction Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernelToRun, Issuccessful);
			if (Issuccessful)
			{
				RunKernelFunction(KernelNumber, DevicesToRunKernel_From, DevicesToRunKernel_To, MultiDeviceNDRange, Issuccessful, MultiDeviceSendStructList);
			}
			else
			{
				//std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernelToRun << " Is Not Found In the Provided Program!\n";
				//std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}

		}

		unsigned int GetTotalNumberOfDevices()
		{
			if (IsConstructionSuccesful)
			{
				return NumberOfDevices;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfDevices Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			return 0;
		}
		unsigned int GetTotalNumberOfKernelFunctions()
		{
			if (IsConstructionSuccesful)
			{
				return TotalNumberOfKernelFunctions;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfKernelFunctions Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			return 0;
		}
		void GetBinaryInformationOfProgram(Essenbp::ArrayOfUnknownDataAndSize& Binary_Program_OfEachDevice, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetBinaryInformationOfProgram Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";				
			}
			else
			{
				cl_int ClErrorResult = 0;		
				
				cl_uint NumDevices;

				ClErrorResult = clGetProgramInfo(SingleProgram, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &NumDevices, NULL);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n ClError Code " << ClErrorResult << " : clGetProgramInfo to get CL_PROGRAM_NUM_DEVICES Failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				else
				{
					size_t* BinarySizes = (size_t*)malloc(NumDevices * sizeof(size_t));
					if (BinarySizes == nullptr)
					{
						std::cout << "\n Error Allocating " << (NumDevices * sizeof(size_t)) << " Bytes for BinarySizes in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else
					{
						ClErrorResult = clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARY_SIZES, NumDevices * sizeof(size_t), BinarySizes, NULL);
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : clGetProgramInfo to get CL_PROGRAM_BINARY_SIZES Failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							free(BinarySizes);
						}
						else
						{
							char** Binaries = new char* [NumDevices];								
							if (Binaries == nullptr)
							{
								std::cout << "\n Error Allocating " << (NumDevices * sizeof(char*)) << " Bytes for Binaries in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
								free(BinarySizes);
							}
							else
							{
								Issuccessful = true;
								for (int i = 0; i < NumDevices; i++)
								{
									Binaries[i] = new char[BinarySizes[i] + 1];
									if (Binaries == nullptr)
									{
										std::cout << "\n Error Allocating " << (NumDevices * sizeof(char*)) << " Bytes for Binaries in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
										for (int j = 0; j < i; j++)
										{
											delete (Binaries[i]);
										}
										delete[] Binaries;
										free(BinarySizes);
										Issuccessful = false;
										break;
									}
								}
								if (Issuccessful)
								{
									ClErrorResult = clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARIES, NumDevices * sizeof(size_t), Binaries, NULL);
									if (ClErrorResult != CL_SUCCESS)
									{
										std::cout << "\n ClError Code " << ClErrorResult << " : clGetProgramInfo to get CL_PROGRAM_BINARIES Failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
										for (int i = 0; i < NumDevices; i++)
										{
											delete (Binaries[i]);
										}
										delete[] Binaries;
										free(BinarySizes);
									}
									else
									{
										Issuccessful = true;
										if (Binary_Program_OfEachDevice.GetTotalNumberOfUnknownData() < NumDevices)
										{
											for (int i = 0; i < NumDevices - Binary_Program_OfEachDevice.GetTotalNumberOfUnknownData(); ++i)
											{
												Binary_Program_OfEachDevice.AddElement(Issuccessful);
												if (!Issuccessful)
												{
													std::cout << "\n Error Binary_Program_OfEachDevice.AddElement failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
													for (int i = 0; i < NumDevices; i++)
													{
														delete (Binaries[i]);
													}
													delete[] Binaries;
													free(BinarySizes);
													Issuccessful = false;
													break;
												}
											}
										}
										if (Issuccessful)
										{
											Essenbp::UnknownDataAndSizeStruct* ReturnValue;
											for (int i = 0; i < NumDevices; i++)
											{
												Binaries[i][BinarySizes[i]] = '\0';
												//std::cout << "Program " << i << ":" << std::endl;
												//std::cout << Binaries[i];													
												Binary_Program_OfEachDevice.GetData(i, &ReturnValue, Issuccessful);
												if (!Issuccessful)
												{
													std::cout << "\n Error Binary_Program_OfEachDevice.GetData failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
													for (int i = 0; i < NumDevices; i++)
													{
														delete (Binaries[i]);
													}
													delete[] Binaries;
													free(BinarySizes);
													Issuccessful = false;
													break;
												}
												ReturnValue->CopyAndStoreData(Binaries[i], BinarySizes[i], Issuccessful);
												if (!Issuccessful)
												{
													std::cout << "\n Error Binary_Program_OfEachDevice.CopyAndStoreData failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
													for (int i = 0; i < NumDevices; i++)
													{
														delete (Binaries[i]);
													}
													delete[] Binaries;
													free(BinarySizes);
													Issuccessful = false;
													break;
												}
											}
										}								
									}
								}
							}								
						}							
					}						
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error GetBinaryInformationOfProgram() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}
		void GetKernelNumberByKernelName(unsigned int& KernelNumber, std::string NameOfTheKernel, bool& Issuccessful)
		{
			Issuccessful = false;
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					if (OrderedKernelArgumentList[i]->KernelFunctionName == NameOfTheKernel)
					{
						Issuccessful = true;
						KernelNumber = i;
						return;
					}
				}
				std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernel << " Is Not Found In the Provided Program!\n";
				std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}
			else
			{
				std::cout << "\n Error Calling GetKernelNumberByKernelName Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
		}
		void GetKernelInformation(unsigned int KernelNumber, cl_KernelFunctionArgumentOrderListStruct** OrderedStruct, bool& Issuccessful)
		{
			Issuccessful = false;
			if (IsConstructionSuccesful)
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					if (OrderedStruct == nullptr)
					{
						std::cout << "\n Error OrderedStruct is nullptr in GetKernelInformation In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else 
					{
						*OrderedStruct = OrderedKernelArgumentList[KernelNumber];
						Issuccessful = true;
					}
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in GetKernelInformation In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}
			else
			{
				std::cout << "\n Error Calling GetKernelInformation Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
		}
		void GetKernelInformation(std::string NameOfTheKernel, cl_KernelFunctionArgumentOrderListStruct** OrderedStruct, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetKernelInformation Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernel, Issuccessful);
			if (Issuccessful)
			{
				GetKernelInformation(KernelNumber, OrderedStruct, Issuccessful);
			}
		}

		void RetreiveDataFromKernel(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, Essenbp::UnknownDataAndSizeStruct& ReteivedData, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RetreiveDataFromKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					if (DeviceNumber < NumberOfDevices)
					{
						if (ArgumentNumber < OrderedKernelArgumentList[KernelNumber]->TotalNumberOfArugments)
						{
							MultiKernelFunction_MultiDevice[KernelNumber]->MultiDeviceKernelArgumentsArray[DeviceNumber]->RetreiveDataFromKernel(ArgumentNumber, BufferNumber, ReteivedData, Issuccessful);
							if (!Issuccessful)
							{
								std::cout << "\n Error cl_KernelMultipleArgumentStruct::RetreiveDataFromKernel() Failed in RetreiveDataFromKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
						}
						else
						{
							std::cout << "\n Error ArgumentNumber Exceeds the Total Number Of Kernel Arguments Present in the Kernel '" << MultiKernelFunction_MultiDevice[KernelNumber]->KernelFunctionName << "'! in RetreiveDataFromKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error DevicesToRunKernel_To Exceeds the Number Of Devices Present! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in RetreiveDataFromKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error RetreiveDataFromKernel() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}
		void RetreiveDataFromKernel(unsigned int DeviceNumber, std::string NameOfTheKernel, unsigned int ArgumentNumber, unsigned int BufferNumber, Essenbp::UnknownDataAndSizeStruct& ReteivedData, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RetreiveDataFromKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernel, Issuccessful);
			if (Issuccessful)
			{
				RetreiveDataFromKernel(DeviceNumber, KernelNumber, ArgumentNumber, BufferNumber, ReteivedData, Issuccessful);
			}
		}

		void InterchangeBufferWithinSameDevice(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, unsigned int TargetKernelNumber, unsigned int TargetArgumentNumber, unsigned int TargetBufferNumber, bool& Issuccessful)//PointerToBuffer = MultiBufferOnDevice[i]
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterChangeBufferWithinSameDevice Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					if (DeviceNumber < NumberOfDevices)
					{
						cl_MemoryStruct* PointerToBuffer;
						MultiKernelFunction_MultiDevice[TargetKernelNumber]->MultiDeviceKernelArgumentsArray[DeviceNumber]->GetPointerToBufferPointer(TargetArgumentNumber, TargetBufferNumber, &PointerToBuffer, Issuccessful);
						MultiKernelFunction_MultiDevice[KernelNumber]->MultiDeviceKernelArgumentsArray[DeviceNumber]->InterchangeBufferWithinSameDevice(ArgumentNumber, BufferNumber, PointerToBuffer, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error cl_KernelSingleArgumentStruct::InterchangeBufferWithinSameDevice() Failed in InterchangeBufferWithinSameDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error DeviceNumber + 1 '" << DeviceNumber + 1 << "' is greater than NumberOfDevices in InterchangeBufferWithSameDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in InterchangeBufferWithinSameDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithinSameDevice() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		void InterchangeBufferWithAnotherDevice(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, unsigned int TargetDeviceNumber, unsigned int TargetKernelNumber, unsigned int TargetArgumentNumber, unsigned int TargetBufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling InterchangeBufferWithAnotherDevice Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			else
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					if (DeviceNumber < NumberOfDevices)
					{
						cl_MemoryStruct* PointerToBuffer;
						MultiKernelFunction_MultiDevice[TargetKernelNumber]->MultiDeviceKernelArgumentsArray[TargetDeviceNumber]->GetPointerToBufferPointer(TargetArgumentNumber, TargetBufferNumber, &PointerToBuffer, Issuccessful);
						MultiKernelFunction_MultiDevice[KernelNumber]->MultiDeviceKernelArgumentsArray[DeviceNumber]->InterchangeBufferWithAnotherDevice(ArgumentNumber, BufferNumber, PointerToBuffer, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error cl_KernelSingleArgumentStruct::InterchangeBufferWithAnotherDevice() Failed in InterchangeBufferWithAnotherDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error DeviceNumber + 1 '" << DeviceNumber + 1 << "' is greater than NumberOfDevices in InterchangeBufferWithAnotherDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in InterchangeBufferWithinSameDevice In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}
			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		void GetBufferNumberBeingUsed(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int& ArgBufferNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling GetBufferNumberBeingUsed Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "\n Error GetBufferNumberBeingUsed() Failed in cl_KernelSingleArgumentStruct!";
			}
			else
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					if (DeviceNumber < NumberOfDevices)
					{
						MultiKernelFunction_MultiDevice[KernelNumber]->MultiDeviceKernelArgumentsArray[DeviceNumber]->GetBufferNumberBeingUsed(ArgumentNumber, ArgBufferNumber, Issuccessful);
						if (!Issuccessful)
						{
							std::cout << "\n Error cl_KernelSingleArgumentStruct::GetBufferNumberBeingUsed() Failed in SetBufferToUse In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
					}
					else
					{
						std::cout << "\n Error DeviceNumber + 1 '" << DeviceNumber + 1 << "' is greater than NumberOfDevices in SetBufferToUse In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in SetBufferToUse In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}

			if (!Issuccessful)// For the safe of readability
			{
				std::cout << "\n Error InterchangeBufferWithAnotherDevice() Failed in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}

		~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct()
		{
			std::cout << "\n Destructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			if (IsConstructionSuccesful)
			{
				ReleaseAndFreeClData(IsConstructionSuccesful);// NOTE:Temporarly used IsCostructionSuccesful Here, Do nothing if Failed
				IsConstructionSuccesful = false;
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					delete MultiKernelFunction_MultiDevice[i];
					delete OrderedKernelArgumentList[i];//Should always be deleted last
				}
				free(MultiKernelFunction_MultiDevice);
				free(OrderedKernelArgumentList);
			}
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: This Is the shortend Name, Does not matter if you use the struct name directly
	typedef cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct OpenCLWrapper;
};
#else
#endif // !OPENCL_WRAPPER_BY_PUNAL