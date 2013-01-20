#include <math.h>
#include "Ball.h"
#include "Brick.h"

Ball::Ball(Texture* texture)
{
    m_isLaunched = false;
    m_texture = texture;
    m_radius = 8.0f;
    m_speed = 4.0f;
    m_velocity = Vector(0.0f, 0.0f, 0.0f);
    m_u1 = m_v2 = 0.0f;
	m_u2 = m_v4 = 0.0f;
	m_u3 = m_v1 = 1.0f;
	m_u4 = m_v3 = 1.0f;
}

void Ball::launch()
{
    if (!m_isLaunched)
    {
        m_isLaunched = true;
        // Start moving!
        m_velocity = Vector(0.0f, -m_speed, 0.0f);
    }
}

float Ball::getWidth() const
{
    float width = m_texture->getW();

	const Vector& sv = getScale();
	width *= sv.x;

    return width;
}

float Ball::getHeight() const
{
    float height = m_texture->getH();

	const Vector& sv = getScale();
	height *= sv.y;

    return height;
}

bool Ball::intersectsWith(Brick* brick) const
{
    const Vector& ballCenter = getPosition();
    const Vector& brickCenter = brick->getPosition();

    float xDistance = fabs(ballCenter.x - brickCenter.x);
    float yDistance = fabs(ballCenter.y - brickCenter.y);

    float halfBrickWidth = brick->getWidth() / 2;
    float halfBrickHeight = brick->getHeight() / 2;

    if (xDistance > halfBrickWidth + m_radius)
    {
        return false;
    }

    if (yDistance > halfBrickHeight + m_radius)
    {
        return false;
    }

    if (xDistance <= halfBrickWidth)
    {
        return true;
    }

    if (yDistance <= halfBrickHeight)
    {
        return true;
    }

    float cornerDistanceSquared = 
        ((xDistance - halfBrickWidth) * (xDistance - halfBrickWidth)) +
        ((yDistance - halfBrickHeight) * (yDistance - halfBrickHeight));

    return (cornerDistanceSquared <= (m_radius * m_radius));
}

void Ball::draw(int list)
{
    if (list != PVR_LIST_TR_POLY || !m_texture)
	{
        return;
    }

	m_texture->sendHdr(list);
    	
	float w = m_texture->getW();
	float h = m_texture->getH();

	const Vector& sv = getScale();
	w *= sv.x;
	h *= sv.y;

    translate(m_velocity);

	const Vector& tv = getPosition();
    
	plx_vertex_t vert;
	vert.argb = getColor();
	vert.oargb = 0;

	vert.flags = PLX_VERT;
	vert.x = tv.x-w/2;
	vert.y = tv.y+h/2;
	vert.z = tv.z;
	vert.u = m_u1;
	vert.v = m_v1;
	plx_prim(&vert, sizeof(vert));

	vert.y = tv.y-h/2;
	vert.u = m_u2;
	vert.v = m_v2;
	plx_prim(&vert, sizeof(vert));

	vert.x = tv.x+w/2;
	vert.y = tv.y+h/2;
	vert.u = m_u3;
	vert.v = m_v3;
	plx_prim(&vert, sizeof(vert));

	vert.flags = PLX_VERT_EOS;
	vert.y = tv.y-h/2;
	vert.u = m_u4;
	vert.v = m_v4;
	plx_prim(&vert, sizeof(vert));
}