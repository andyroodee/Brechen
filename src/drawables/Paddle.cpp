#include "Paddle.h"

Paddle::Paddle()
{
}

void Paddle::setSize(float w, float h) 
{
	m_width = w;
	m_height = h;
}

void Paddle::draw(int list)
{
    if (list != PVR_LIST_TR_POLY)
    {
		return;
    }

    const Vector& pos = getPosition();

    pvr_poly_hdr_t hdr;
    pvr_poly_cxt_t cxt;
    pvr_vertex_t vert;

    pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.flags = PVR_CMD_VERTEX;
    vert.x = pos.x - m_width/2;
    vert.y = pos.y + m_height/2;
    vert.z = 10.0f;
    vert.u = 0;
    vert.v = 0;
    vert.argb = getColor();
    vert.oargb = 0;
    pvr_prim(&vert, sizeof(vert));

    vert.y = pos.y - m_height/2;
    pvr_prim(&vert, sizeof(vert));
 
    vert.x = pos.x + m_width/2;
    vert.y = pos.y + m_height/2;
    pvr_prim(&vert, sizeof(vert));

    vert.flags = PVR_CMD_VERTEX_EOL;
    vert.y = pos.y - m_height/2;
    pvr_prim(&vert, sizeof(vert));
}