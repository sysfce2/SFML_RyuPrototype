#include <Ryu/Physics/DebugDraw.h>

#include <iostream>

DebugDraw g_debugDraw;

DebugDraw::DebugDraw() {}

DebugDraw::~DebugDraw() {}

void 
DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	std::cout << "draw polygon " << "\n";
}

void
DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{}

void
DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{}

void
DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{}

void
DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{}

void
DebugDraw::DrawTransform(const b2Transform& xf)
{}

void
DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{}

void
DebugDraw::DrawString(int x, int y, const char* string, ...)
{} 

void
DebugDraw::DrawString(const b2Vec2& p, const char* string, ...)
{}

void
DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{}

void
DebugDraw::Flush()
{}
