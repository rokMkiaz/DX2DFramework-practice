#pragma once

#define NOMINMAX

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console" )
#endif 

//Window
#include<Windows.h>
#include<assert.h> 


//STL
#include<iostream>
#include<vector>

//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")


typedef unsigned int  uint;

//Macro Function
#define SAFE_DELETE(p)       { if(p) { delete(p); (p)=nullptr;     }  }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=nullptr;  }  }
#define SAFE_RELEASE(p)      { if(p) {(p)->Release(); (p)=nullptr; }  }

//Framework
#include"Settings.h"
#include"Graphics.h"

#include"Vertex\D3D11_Vertex.h"
#include"Vertex\D3D11_Geometry.h"
#include"InputAssembler\D3D11_VertexBuffer.h"
#include"InputAssembler\D3D11_IndexBuffer.h"
#include"InputAssembler\D3D11_InputLayout.h"
#include"Shader\D3D11_Shader.h"
#include"Shader\D3D11_ConstantBuffer.h"
#include"Shader\D3D11_SamplerState.h"
#include"Shader\D3D11_Texture.h"
#include"Rasterizer\D3D11RasterizerState.h"
#include"Rasterizer\D3D11_Viewport.h"

