#include "Border.h"

Border::Border()
{
    const Color dark = Color(0.6f, 0.6f, 1.0f);
    const Color light = Color(0.4f, 0.4f, 0.6f);
    
    leftBorderCoords[0] = Vector(LEFT_BORDER - BORDER_SIZE, BOTTOM_BORDER, 0.0f);
    leftBorderCoords[1] = Vector(LEFT_BORDER - BORDER_SIZE, TOP_BORDER - BORDER_SIZE, 0.0f);
    leftBorderCoords[2] = Vector(LEFT_BORDER,               BOTTOM_BORDER, 0.0f);
    leftBorderCoords[3] = Vector(LEFT_BORDER,               TOP_BORDER, 0.0f);
    leftBorderColours[0] = leftBorderColours[1] = dark;
    leftBorderColours[2] = leftBorderColours[3] = light;
    
    rightBorderCoords[0] = Vector(RIGHT_BORDER,                 BOTTOM_BORDER, 0.0f);
    rightBorderCoords[1] = Vector(RIGHT_BORDER,                 TOP_BORDER, 0.0f);
    rightBorderCoords[2] = Vector(RIGHT_BORDER + BORDER_SIZE,   BOTTOM_BORDER, 0.0f);
    rightBorderCoords[3] = Vector(RIGHT_BORDER + BORDER_SIZE,   TOP_BORDER - BORDER_SIZE, 0.0f);
    rightBorderColours[0] = rightBorderColours[1] = light;
    rightBorderColours[2] = rightBorderColours[3] = dark;

    topBorderCoords[0] = Vector(LEFT_BORDER,                TOP_BORDER, 0.0f);
    topBorderCoords[1] = Vector(LEFT_BORDER - BORDER_SIZE,  TOP_BORDER - BORDER_SIZE, 0.0f);
    topBorderCoords[2] = Vector(RIGHT_BORDER,               TOP_BORDER, 0.0f);
    topBorderCoords[3] = Vector(RIGHT_BORDER + BORDER_SIZE, TOP_BORDER - BORDER_SIZE, 0.0f);
    topBorderColours[0] = topBorderColours[2] = light;
    topBorderColours[1] = topBorderColours[3] = dark;
}

void Border::draw(int list)
{
    if (list != PVR_LIST_TR_POLY)
    {
		return;
    }
    
    pvr_poly_hdr_t hdr;
    pvr_poly_cxt_t cxt;

    pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));   
    
    drawQuad(leftBorderCoords, leftBorderColours);
    drawQuad(rightBorderCoords, rightBorderColours);
    drawQuad(topBorderCoords, topBorderColours);
}

void Border::drawQuad(Vector coords[4], Color colours[4])
{
    pvr_vertex_t vert;

    vert.flags = PVR_CMD_VERTEX;
    vert.z = 10.0f;
    vert.u = 0;
    vert.v = 0;
    vert.oargb = 0;

    vert.x = coords[0].x;
    vert.y = coords[0].y;
    vert.argb = colours[0];
    pvr_prim(&vert, sizeof(vert));

    vert.x = coords[1].x;
    vert.y = coords[1].y;    
    vert.argb = colours[1];
    pvr_prim(&vert, sizeof(vert));
 
    vert.x = coords[2].x;
    vert.y = coords[2].y;
    vert.argb = colours[2];
    pvr_prim(&vert, sizeof(vert));

    vert.flags = PVR_CMD_VERTEX_EOL;
    vert.x = coords[3].x;
    vert.y = coords[3].y;  
    vert.argb = colours[3];
    pvr_prim(&vert, sizeof(vert));
}