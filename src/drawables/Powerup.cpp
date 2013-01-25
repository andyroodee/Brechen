#include "Powerup.h"
#include "Border.h"

Powerup::Powerup(Effect effect, const Vector& position)
{
    m_effect = effect;
    m_u1 = m_v2 = 0.0f;
    m_u2 = m_v4 = 0.0f;
    m_u3 = m_v1 = 1.0f;
    m_u4 = m_v3 = 1.0f;
    m_velocity = Vector(0.0f, 2.0f, 0.0f);
    loadTexture();
    setTranslate(position);
}

void Powerup::loadTexture()
{
    char texturePath[256];
    switch (m_effect)
    {
    case ExtendPaddle:
        strcpy(texturePath, "/rd/powerups/extend_paddle.png");
        break;
    case ExtraBalls:
        strcpy(texturePath, "/rd/powerups/extra_balls.png");
        break;
    case Powerball:
        strcpy(texturePath, "/rd/powerups/power_ball.png");
        break;
    case LaserPaddle:
        strcpy(texturePath, "/rd/powerups/laser_paddle.png");
        break;
    case ShrinkPaddle:
        strcpy(texturePath, "/rd/powerups/shrink_paddle.png");
        break;
    case DoubleBallSpeed:
        strcpy(texturePath, "/rd/powerups/double_ball_speed.png");
        break;
    case RandomBounce:
        strcpy(texturePath, "/rd/powerups/random_bounce.png");
        break;
    default:
        break;
    }

    m_texture = new Texture(texturePath, true);
    m_texture->setFilter(Texture::FilterNone);
}

float Powerup::getWidth() const
{
    float width = m_texture->getW();

	const Vector& sv = getScale();
	width *= sv.x;

    return width;
}

float Powerup::getHeight() const
{
    float height = m_texture->getH();

	const Vector& sv = getScale();
	height *= sv.y;

    return height;
}

void Powerup::draw(int list)
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

    if (tv.y > Border::BOTTOM_BORDER)
    {
        setFinished();
    }
        
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