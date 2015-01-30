#include "WSEEngineOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void SetShaderParamInt(WSEEngineOperationsContext *context)
{
#if defined WARBAND
	rgl::string parameter_name;
	int value;

	context->ExtractString(parameter_name);
	context->ExtractValue(value);
	
	parameter_name.spaces_to_underscores();

	if (!warband->hlsl_mode)
		return;

	D3DXHANDLE handle = warband->effect->GetParameterByName(nullptr, parameter_name.c_str());

	if (!handle)
		context->ScriptError("invalid handle for shader parameter %s", parameter_name.c_str());

	warband->effect->SetInt(handle, value);
#endif
}

void SetShaderParamFloat(WSEEngineOperationsContext *context)
{
#if defined WARBAND
	rgl::string parameter_name;
	float value;

	context->ExtractString(parameter_name);
	context->ExtractFixedPoint(value);
	
	parameter_name.spaces_to_underscores();

	if (!warband->hlsl_mode)
		return;

	D3DXHANDLE handle = warband->effect->GetParameterByName(nullptr, parameter_name.c_str());

	if (!handle)
		context->ScriptError("invalid handle for shader parameter %s", parameter_name.c_str());

	warband->effect->SetFloat(handle, value);
#endif
}

void SetShaderParamVector(WSEEngineOperationsContext *context)
{
#if defined WARBAND
	rgl::string parameter_name;
	int preg;

	context->ExtractString(parameter_name);
	context->ExtractRegister(preg);
	
	parameter_name.spaces_to_underscores();

	if (!warband->hlsl_mode)
		return;

	D3DXHANDLE handle = warband->effect->GetParameterByName(nullptr, parameter_name.c_str());

	if (!handle)
		context->ScriptError("invalid handle for shader parameter %s", parameter_name.c_str());

	D3DXVECTOR4 vector;
	
	memcpy(&vector, &warband->basic_game.position_registers[preg].o, sizeof(D3DXVECTOR4));
	warband->effect->SetVector(handle, &vector);
#endif
}

void SetShaderParamMatrix(WSEEngineOperationsContext *context)
{
#if defined WARBAND
	rgl::string parameter_name;
	int preg;

	context->ExtractString(parameter_name);
	context->ExtractRegister(preg);
	
	parameter_name.spaces_to_underscores();

	if (!warband->hlsl_mode)
		return;

	D3DXHANDLE handle = warband->effect->GetParameterByName(nullptr, parameter_name.c_str());

	if (!handle)
		context->ScriptError("invalid handle for shader parameter %s", parameter_name.c_str());

	D3DXMATRIX matrix;
	
	memcpy(&matrix, &warband->basic_game.position_registers[preg], sizeof(D3DXMATRIX));
	matrix._14 = 0.0f;
	matrix._24 = 0.0f;
	matrix._34 = 0.0f;
	matrix._44 = 1.0f;
	warband->effect->SetMatrixTranspose(handle, &matrix);
#endif
}
	
void UpdateMaterial(WSECoreOperationsContext *context)
{
#if defined WARBAND
	rgl::string m1n, m2n;
	
	context->ExtractString(m1n);
	context->ExtractString(m2n);
	
	m1n.spaces_to_underscores();
	m2n.spaces_to_underscores();
	rgl::material *m1 = NULL;
	rgl::material *m2 = NULL;

	for (int i = 0; i < warband->resource_manager.materials.size() && (!m1 || !m2); ++i)
	{
		if (warband->resource_manager.materials[i]->name == m1n)
			m1 = warband->resource_manager.materials[i];
		else if (warband->resource_manager.materials[i]->name == m2n)
			m2 = warband->resource_manager.materials[i];
	}
	
	if (!m1)
		context->ScriptError("Invalid material %s", m1n.c_str());

	if (!m2)
		context->ScriptError("Invalid material %s", m2n.c_str());
	
	if (warband->rendering_event_1)
	{
		LeaveCriticalSection(&warband->multithreading_critical_section);
		EnterCriticalSection(&warband->directx_critical_section);
		EnterCriticalSection(&warband->multithreading_critical_section);
	}
		
	m1->shader = m2->shader;
	m1->shader_name = m2->shader_name;
	m1->specular_coefficient = m2->specular_coefficient;
	m1->specular_color = m2->specular_color;
	m1->textures[0] = m2->textures[0];
	m1->textures[1] = m2->textures[1];
	m1->textures[2] = m2->textures[2];
	m1->textures[3] = m2->textures[3];
	m1->textures[4] = m2->textures[4];

	if (warband->rendering_event_1)
		LeaveCriticalSection(&warband->directx_critical_section);
#endif
}

WSEEngineOperationsContext::WSEEngineOperationsContext() : WSEOperationContext("engine", 4700, 4799)
{
}

void WSEEngineOperationsContext::OnLoad()
{
	RegisterOperation("set_shader_param_int", SetShaderParamInt, Client, None, 2, 2,
		"Sets the int shader parameter <0> to <1>",
		"parameter_name", "value");

	RegisterOperation("set_shader_param_float", SetShaderParamFloat, Client, None, 2, 2,
		"Sets the float shader parameter <0> to <1>",
		"parameter_name", "value_fixed_point");

	RegisterOperation("set_shader_param_vector", SetShaderParamVector, Client, None, 2, 2,
		"Sets the float4 shader parameter <0> to <1>'s origin",
		"parameter_name", "position_register");

	RegisterOperation("set_shader_param_matrix", SetShaderParamMatrix, Client, None, 2, 2,
		"Sets the float4x4 shader parameter <0> to <1>",
		"parameter_name", "position_register");
	
	RegisterOperation("update_material", UpdateMaterial, Client, None, 2, 2,
		"Updates <0> with <1>",
		"material_name", "new_material_name");
}
