#pragma once

#include "rgl_pool.h"
#include "rgl_vector4.h"
#include "stl_priority_queue.h"
#include "stl_vector.h"

namespace rgl
{
	struct ai_mesh_vertex;
	struct ai_mesh_edge;
	struct ai_mesh_face;

	template <class T>
	struct ai_mesh_container
	{
		T *head;
		T *tail;
		pool<T> *pool;
		int count;
	};

	struct ai_mesh_vertex
	{
		ai_mesh_vertex *next;
		vector4 position;
		bool selected;
		int value;
	};

	struct ai_mesh_edge
	{
		ai_mesh_edge *next;
		ai_mesh_vertex *vertices[2];
		ai_mesh_face *faces[2];
		int num_faces;
		bool selected;
		float steepness[2];
		float traverse_cost[2][2];
		int value;
		vector4 center;
	};

	struct ai_mesh_face
	{
		ai_mesh_edge *edges[4];
		ai_mesh_face *next;
		int color;
		ai_mesh_face *next_in_path;
		vector4 center;
		vector4 normal;
		ai_mesh_vertex *vertices[4];
		int num_vertices;
		ai_mesh_face *neighbor_faces[4];
		int u1;
		bool selected;
		int value;
		bool visible;
	};

	struct ai_mesh_path_finding_point
	{
		ai_mesh_face *face;
		float distance;
		float total_distance;
	};

	struct ai_mesh_path_checking_point
	{
		ai_mesh_face *face;
		float distance;
		int num_steps;
	};

	struct ai_mesh
	{
		int white_point;
		stl::priority_queue<ai_mesh_path_finding_point> path_finding_points;
		stl::priority_queue<ai_mesh_path_checking_point> path_checking_points;
		ai_mesh_container<ai_mesh_vertex> vertices;
		ai_mesh_container<ai_mesh_edge> edges;
		ai_mesh_container<ai_mesh_face> faces;
		int selection_type;
		int selection_object;
		int u1;
		int u2;
		int u3;
		int u4;
		int u5;
		int u6;
		vector4 u7;
	};
}
