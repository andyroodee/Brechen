#include "Brick.h"

Brick::Brick()
{
}

void Brick::setType(unsigned char type)
{
    m_type = type;
    // Colour based on type. 
    switch (m_type)
    {
    case '1':
        setTint(Color(1.0f, 0.0f, 1.0f, 0.0f));
        break;
    case '2':
        setTint(Color(1.0f, 0.0f, 0.0f, 1.0f));
        break;
    case '3':
        setTint(Color(1.0f, 1.0f, 0.0f, 0.0f));
        break;
    default:
        setTint(Color(0.0f, 0.0f, 0.0f, 0.0f));
        break;
    }
}

void Brick::draw(int list)
{
    if (list != PVR_LIST_TR_POLY || 
        m_type == '0')
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
    vert.x = pos.x - BRICK_WIDTH/2;
    vert.y = pos.y + BRICK_HEIGHT/2;
    vert.z = 10.0f;
    vert.u = 0;
    vert.v = 0;
    vert.argb = getColor();
    vert.oargb = 0;
    pvr_prim(&vert, sizeof(vert));

    vert.y = pos.y - BRICK_HEIGHT/2;
    pvr_prim(&vert, sizeof(vert));
 
    vert.x = pos.x + BRICK_WIDTH/2;
    vert.y = pos.y + BRICK_HEIGHT/2;
    pvr_prim(&vert, sizeof(vert));

    vert.flags = PVR_CMD_VERTEX_EOL;
    vert.y = pos.y - BRICK_HEIGHT/2;
    pvr_prim(&vert, sizeof(vert));
}