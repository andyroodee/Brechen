#include "Brick.h"
#include <tsu/anims/logxymover.h>
#include <tsu/triggers/death.h>

Brick::Brick(int width, int height)
{
    m_width = width;
    m_height = height;
    m_value = 20;
    m_alive = true;
}

void Brick::animateDestruction(const Vector& direction, const float power)
{
    m_alive = false;
    const Vector& pos = getPosition();
    Vector push = direction.normalize();
    push *= power;
    RefPtr<LogXYMover> mover = new LogXYMover(pos.x + push.x, pos.y + push.y);
    mover->triggerAdd(new Death(this));
    animAdd(mover);
}

void Brick::draw(int list)
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
    
    Color light = getColor() * 0.8f;
    light.a = 1.0f;

    Color dark = getColor() * 1.2f;
    dark.a = 1.0f;
    
    vert.flags = PVR_CMD_VERTEX;
    vert.x = pos.x - m_width/2 + BORDER_SIZE;
    vert.y = pos.y + m_height/2 - BORDER_SIZE;
    vert.z = 10.0f;
    vert.u = 0;
    vert.v = 0;
    vert.argb = light;
    vert.oargb = 0;
    pvr_prim(&vert, sizeof(vert));

    vert.y = pos.y - m_height/2 + BORDER_SIZE;    
    vert.argb = dark;
    pvr_prim(&vert, sizeof(vert));
 
    vert.x = pos.x + m_width/2 - BORDER_SIZE;
    vert.y = pos.y + m_height/2 - BORDER_SIZE;
    vert.argb = getColor();
    pvr_prim(&vert, sizeof(vert));

    vert.flags = PVR_CMD_VERTEX_EOL;
    vert.y = pos.y - m_height/2 + BORDER_SIZE;    
    pvr_prim(&vert, sizeof(vert));

	Drawable::draw(list);
}