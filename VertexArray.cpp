#include "VertexArray.hpp"
#include <stdlib.h>

bool Npi::VertexArray::lockCreate = false;
u32 Npi::VertexArray::currIndex = 0;

Npi::VertexArray* Npi::VertexArray::Create()
{
	if (lockCreate)
		return nullptr;
	lockCreate = true;
	return new VertexArray(currIndex);
}

void Npi::VertexArray::Dispose(VertexArray* array)
{
	if (array)
		delete array;
}

void Npi::VertexArray::AddVertex(const Npi::Polygon::Vertex& vertex)
{
	if (!lockCreate)
		return;
	vbo_data[startIndex + size++] = vertex;
}

static Npi::Polygon::Vertex defaultV;
Npi::Polygon::Vertex& Npi::VertexArray::GetVertex(u32 index)
{
	if (index < size)
		return vbo_data[startIndex + index];
	else
		return defaultV;
}

u32 Npi::VertexArray::Size()
{
	return size;
}

void Npi::VertexArray::Complete()
{
	lockCreate = false;
	currIndex = startIndex + size;
	if (currIndex > MAX_VERTEX_COUNT)
		abort();
}

void Npi::VertexArray::draw(Npi::RenderContext t_context)
{
	C3D_DrawArrays(t_context.GetPrimitiveType(), startIndex, size);
}