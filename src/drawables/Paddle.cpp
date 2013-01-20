#include "Paddle.h"

Paddle::Paddle(Texture* texture)
{
    m_texture = texture;
    m_u1 = m_v2 = 0.0f;
	m_u2 = m_v4 = 0.0f;
	m_u3 = m_v1 = 1.0f;
	m_u4 = m_v3 = 1.0f;
	m_width = m_height = -1.0f;
}

void Paddle::draw(int list)
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