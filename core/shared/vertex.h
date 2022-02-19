
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <type_traits>

namespace core
{
	enum class RHI_Vertex_Type : uint8_t
	{
		RHI_Vertex_Type_Unknown,
		RHI_Vertex_Type_Position,
		RHI_Vertex_Type_PositionColor,
		RHI_Vertex_Type_PositionTexture,
		RHI_Vertex_Type_PositionTextureNormalTangent,
		RHI_Vertex_Type_Position2dTextureColor8
	};

	struct RHI_Vertex_Undefined {};

	struct RHI_Vertex_Pos
	{
		float pos[3] = { 0 };
	};

	struct RHI_Vertex_PosTex
	{
		float pos[3] = { 0 };
		float tex[2] = { 0 };
	};

	struct RHI_Vertex_PosCol
	{
		float pos[3] = { 0 };
		float col[4] = { 0 };
	};

	struct RHI_Vertex_Pos2dTexCol8
	{
		RHI_Vertex_Pos2dTexCol8() = default;

		float pos[2] = { 0 };
		float tex[2] = { 0 };
		uint32_t col = 0;
	};

	struct RHI_Vertex_PosTexNorTan
	{
		RHI_Vertex_PosTexNorTan() = default;

		float pos[3] = { 0 };
		float tex[2] = { 0 };
		float nor[3] = { 0 };
		float tan[3] = { 0 };
	};

	static_assert(std::is_trivially_copyable<RHI_Vertex_Pos>::value, "RHI_Vertex_Pos is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosTex>::value, "RHI_Vertex_PosTex is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosCol>::value, "RHI_Vertex_PosCol is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_Pos2dTexCol8>::value, "RHI_Vertex_Pos2dTexCol8 is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosTexNorTan>::value, "RHI_Vertex_PosTexNorTan is not trivially copyable");
}
