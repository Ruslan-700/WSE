#pragma once

#include "d3dx9.h"
#include "rgl_hlsl_technique.h"
#include "rgl_matrix.h"
#include "rgl_material.h"
#include "rgl_mesh.h"
#include "rgl_postfx_node.h"
#include "rgl_render_context.h"
#include "rgl_shader.h"
#include "rgl_string.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

#define NUM_VDTS 6
#define NUM_VB_POLICIES 2

namespace rgl
{
	enum vertex_declaration_type
	{
		vdt_regular_ffp         = 0,
		vdt_regular             = 1,
		vdt_normal_map          = 2,
		vdt_skinning            = 3,
		vdt_normal_map_skinning = 4,
		vdt_special             = 5,
	};

	enum vertex_buffer_policy
	{
		vbp_static  = 0,
		vbp_dynamic = 1,
	};

	enum vertex_buffer_flags
	{
		vbf_has_normal         = 0x1,
		vbf_has_color          = 0x2,
		vbf_has_uv             = 0x10,
		vbf_has_specular_color = 0x40,
		vbf_has_tangents       = 0x100,
		vbf_has_skin_data      = 0x1000,
		vbf_has_position       = 0x1000000,
	};

	struct vertex_buffer
	{
		int vertex_size;
		unsigned int flags;
		int last_pos;
		int num_uses;
		string type;
		int capacity;
		int policy;
		stl::vector<mesh *> meshes;
		IDirect3DVertexBuffer9 *dx_vertex_buffer;
		IDirect3DIndexBuffer9 *dx_index_buffer;
		IDirect3DVertexDeclaration9 *dx_vertex_declaration;
		IDirect3DVertexDeclaration9 *dx_vertex_declaration_instanced;
		int total_used_capacity;
		int used_capacity_in_last_frame;
		bool manually_discarded;
	};

	struct renderer_mesh_collection
	{
		render_mesh *meshes[256];
	};

	struct renderer
	{
		IDirect3DVertexDeclaration9 *dx_default_vertex_declaration;
		IDirect3DVertexDeclaration9 *dx_vertex_declarations[NUM_VDTS];
		bool stream_source_used;
		int stream_source_num_instances;
		int instance_buffer[0x60000];
		int instance_buffer_pos;
		stl::vector<vertex_buffer *> vertex_buffers[NUM_VDTS];
		vertex_buffer index_buffers[NUM_VB_POLICIES];
		int vertex_buffer_sizes[NUM_VDTS * NUM_VB_POLICIES];
		IDirect3DVertexDeclaration9 *dx_active_vertex_declaration;
		int active_vertex_buffer_policy;
		int active_index_buffer_policy;
		int active_vertex_buffer_declaration_no;
		int active_vertex_buffer_index;
		int buffer[0x418000];
		shader *active_shader;
		D3DXHANDLE active_technique;
		material *active_material;
		matrix *world_matrices;
		mesh unused_mesh;
		render_context unused_render_context;
		shader *render_to_depth_shader;
		shader *render_to_depth_skin_shader;
		shader *render_to_depth_biased_shader;
		shader *render_character_shadow_shader;
		shader *render_character_shadow_with_skin_shader;
		shader *render_to_light_depth_shader;
		shader *render_to_light_depth_skin_shader;
		texture *screen_texture;
		int reflections_render_no;
		D3DXHANDLE dx_earlyz_technique_handles[8];
		render_context cur_render_contexts[64];
		stl::vector<renderer_mesh_collection *> mesh_collections;
		int mesh_collection_no;
		int mesh_collection_mesh_no;
		stl::vector<render_context *> render_contexts;
		int num_refreshes;
		ID3DXEffect *dx_earlyz_effect;
		ID3DXEffect *dx_postfx_effect;
		IDirect3DVertexDeclaration9 *postfx_vertex_declaration;
		D3DXHANDLE postfx_texture_handles[5];
		IDirect3DVertexBuffer9 *dx_mesh_instancing_vertex_buffer;
		int postfx_needs_restore;
		postfx_node scene_converted_node;
		postfx_node bright_pass_node;
		postfx_node blur_x_1_node;
		postfx_node blur_y_1_node;
		postfx_node blur_x_2_node;
		postfx_node blur_y_2_node;
		int hdr_nodes[7];
		postfx_node dof_blur_node;
		postfx_node average_node_64;
		postfx_node average_node_16;
		postfx_node average_node_4;
		postfx_node luminance_nodes[2];
		postfx_node final_node;
		int autoexp_nodes[7];
		int cur_luminance_node;
		stl::vector<postfx_node *> postfx_nodes;
		vector4 postfx_params[4];
		vector4 postfx_param_mults[4];
		int u1;
		postfx_node u2;
		int u3[2];
		int dof_hdr_antialiasing_samples;
		bool in_postfx_rendering_mode;
		IDirect3DSurface9 *dx_render_target_surface;
		IDirect3DSurface9 *dx_depth_stencil_surface;
		texture *render_target_texture;
		texture *depth_stencil_texture;
		D3DCUBEMAP_FACES render_target_cube_map_faces;
	};
}
