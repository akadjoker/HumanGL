
#include <iostream>
#include <fstream>
#include <sstream>
#include "Batch.hpp"
#include "Math.hpp"
#include "Device.hpp"


#define MAX_TEXT_BUFFER_LENGTH              1024    

#define BATCH_DRAWCALLS 256

#define LINES 0x0001
#define TRIANGLES 0x0004
#define QUAD 0x0008

RenderBatch::RenderBatch()
{

    colorr = 255;
    colorg = 255;
    colorb = 255;
    colora = 255;
    texcoordx = 0.0f;
    texcoordy = 0.0f;
    currentBuffer = 0;
    vertexCounter = 0;
    currentDepth = -1.0f;
    defaultTextureId = 0;
    bufferCount = 0;
    drawCounter = 1;
    use_matrix = false;
  
}
void RenderBatch::Init(int numBuffers, int bufferElements)
{

    unsigned char pixels[4] = {255, 255, 255, 255};
    Pixmap image(1, 1, 4, pixels);
    m_defaultTexture.Load(image);
    defaultTextureId = m_defaultTexture.GetID();

    if (defaultTextureId != 0)
        LogInfo("BATCH: [ID %i] Default texture loaded successfully", defaultTextureId);
    else
        LogError("BATCH: Failed to load default texture [%d] ", defaultTextureId);

    for (int i = 0; i < numBuffers; i++)
    {
        vertexBuffer.push_back(new VertexBuffer());
    }

    vertexCounter = 0;
    currentBuffer = 0;

    for (int i = 0; i < numBuffers; i++)
    {
        vertexBuffer[i]->elementCount = bufferElements;

        int k = 0;

        for (int j = 0; j <= bufferElements; j++)
        {

            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->colors.push_back(colorr);
            vertexBuffer[i]->colors.push_back(colorg);
            vertexBuffer[i]->colors.push_back(colorb);
            vertexBuffer[i]->colors.push_back(colora);

            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->colors.push_back(colorr);
            vertexBuffer[i]->colors.push_back(colorg);
            vertexBuffer[i]->colors.push_back(colorb);
            vertexBuffer[i]->colors.push_back(colora);

            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->colors.push_back(colorr);
            vertexBuffer[i]->colors.push_back(colorg);
            vertexBuffer[i]->colors.push_back(colorb);
            vertexBuffer[i]->colors.push_back(colora);

            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->vertices.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->texcoords.push_back(0.0f);
            vertexBuffer[i]->colors.push_back(colorr);
            vertexBuffer[i]->colors.push_back(colorg);
            vertexBuffer[i]->colors.push_back(colorb);
            vertexBuffer[i]->colors.push_back(colora);

            vertexBuffer[i]->indices.push_back(k);
            vertexBuffer[i]->indices.push_back(k + 1);
            vertexBuffer[i]->indices.push_back(k + 2);
            vertexBuffer[i]->indices.push_back(k);
            vertexBuffer[i]->indices.push_back(k + 2);
            vertexBuffer[i]->indices.push_back(k + 3);

            k += 4;
        }
    }

    for (int i = 0; i < numBuffers; i++)
    {
        glGenVertexArrays(1, &vertexBuffer[i]->vaoId);
        glBindVertexArray(vertexBuffer[i]->vaoId);

        glGenBuffers(1, &vertexBuffer[i]->vboId[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]->vboId[0]);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer[i]->vertices.size() * sizeof(float), vertexBuffer[i]->vertices.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

        glGenBuffers(1, &vertexBuffer[i]->vboId[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]->vboId[1]);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer[i]->texcoords.size() * sizeof(float), vertexBuffer[i]->texcoords.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

        glGenBuffers(1, &vertexBuffer[i]->vboId[2]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]->vboId[2]);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer[i]->colors.size() * sizeof(unsigned char), vertexBuffer[i]->colors.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

        glGenBuffers(1, &vertexBuffer[i]->vboId[3]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer[i]->vboId[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer[i]->indices.size() * sizeof(unsigned int), vertexBuffer[i]->indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    for (int i = 0; i < BATCH_DRAWCALLS; i++)
    {
        draws.push_back(new DrawCall());
        draws[i]->mode = QUAD;
        draws[i]->vertexCount = 0;
        draws[i]->vertexAlignment = 0;
        draws[i]->textureId = defaultTextureId;
    }

    bufferCount = numBuffers; // Record buffer count
    drawCounter = 1;          // Reset draws counter
    currentDepth = -1.0f;     // Reset depth value
}

void UnloadVertexArray(unsigned int vaoId)
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vaoId);
}

void RenderBatch::Release()
{
    if (vertexBuffer.size() == 0)
        return;

    for (int i = 0; i < (int)vertexBuffer.size(); i++)
    {
        glDeleteBuffers(1, &vertexBuffer[i]->vboId[0]);
        glDeleteBuffers(1, &vertexBuffer[i]->vboId[1]);
        glDeleteBuffers(1, &vertexBuffer[i]->vboId[2]);
        glDeleteBuffers(1, &vertexBuffer[i]->vboId[3]);
        UnloadVertexArray(vertexBuffer[i]->vaoId);
    }
    for (int i = 0; i < (int)draws.size(); i++)
    {

        delete draws[i];
    }
    for (int i = 0; i < (int)vertexBuffer.size(); i++)
    {
        vertexBuffer[i]->vertices.clear();
        vertexBuffer[i]->texcoords.clear();
        vertexBuffer[i]->colors.clear();
        vertexBuffer[i]->indices.clear();

        delete vertexBuffer[i];
    }
    draws.clear();
    vertexBuffer.clear();
    m_defaultTexture.Release();
    LogInfo("Render batch  unloaded successfully from VRAM (GPU)");
}

RenderBatch::~RenderBatch()
{
    Release();
}

void RenderBatch::Render()
{
    if (vertexCounter > 0)
    {
        glBindVertexArray(vertexBuffer[currentBuffer]->vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[currentBuffer]->vboId[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCounter * 3 * sizeof(float), vertexBuffer[currentBuffer]->vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[currentBuffer]->vboId[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCounter * 2 * sizeof(float), vertexBuffer[currentBuffer]->texcoords.data());

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[currentBuffer]->vboId[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCounter * 4 * sizeof(unsigned char), vertexBuffer[currentBuffer]->colors.data());

        glBindVertexArray(0);
    }
    if (vertexCounter > 0)
    {

        glBindVertexArray(vertexBuffer[currentBuffer]->vaoId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer[currentBuffer]->vboId[3]);
        glActiveTexture(GL_TEXTURE0);

        for (int i = 0, vertexOffset = 0; i < drawCounter; i++)
        {

            glBindTexture(GL_TEXTURE_2D, draws[i]->textureId);
            int mode = GL_LINES;
            if (draws[i]->mode == LINES)
                mode = GL_LINES;
            else if (draws[i]->mode == TRIANGLES)
                mode = GL_TRIANGLES;
            else if (draws[i]->mode == QUAD)
                mode = GL_TRIANGLES;

            if ((draws[i]->mode == LINES) || (draws[i]->mode == TRIANGLES))
            {
                glDrawArrays(mode, vertexOffset, draws[i]->vertexCount);
            }
            else
            {
                glDrawElements(GL_TRIANGLES, draws[i]->vertexCount / 4 * 6, GL_UNSIGNED_INT, (GLvoid *)(vertexOffset / 4 * 6 * sizeof(int)));
            }

            vertexOffset += (draws[i]->vertexCount + draws[i]->vertexAlignment);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(0); // Unbind VAO
    vertexCounter = 0;
    currentDepth = -1.0f;
    for (int i = 0; i < BATCH_DRAWCALLS; i++)
    {
        draws[i]->mode = QUAD;
        draws[i]->vertexCount = 0;
        draws[i]->textureId = defaultTextureId;
    }
    drawCounter = 1;
    currentBuffer++;
    if (currentBuffer >= bufferCount)
        currentBuffer = 0;
}

void RenderBatch::Line3D(float startX, float startY, float startZ, float endX, float endY, float endZ)
{
    SetMode(LINES);
    Vertex3f(startX, startY, startZ);
    Vertex3f(endX, endY, endZ);
}

bool RenderBatch::CheckRenderBatchLimit(int vCount)
{
    bool overflow = false;

    if ((vertexCounter + vCount) >= (vertexBuffer[currentBuffer]->elementCount * 4))
    {
        overflow = true;

        // Store current primitive drawing mode and texture id
        int currentMode = draws[drawCounter - 1]->mode;
        int currentTexture = draws[drawCounter - 1]->textureId;

        Render();

        // Restore state of last batch so we can continue adding vertices
        draws[drawCounter - 1]->mode = currentMode;
        draws[drawCounter - 1]->textureId = currentTexture;
    }

    return overflow;
}

void RenderBatch::SetMode(int mode)
{
    if (draws[drawCounter - 1]->mode != mode)
    {
        if (draws[drawCounter - 1]->vertexCount > 0)
        {
            if (draws[drawCounter - 1]->mode == LINES)
                draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4) ? draws[drawCounter - 1]->vertexCount : draws[drawCounter - 1]->vertexCount % 4);
            else if (draws[drawCounter - 1]->mode == TRIANGLES)
                draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4) ? 1 : (4 - (draws[drawCounter - 1]->vertexCount % 4)));
            else
                draws[drawCounter - 1]->vertexAlignment = 0;

            if (!CheckRenderBatchLimit(draws[drawCounter - 1]->vertexAlignment))
            {
                vertexCounter += draws[drawCounter - 1]->vertexAlignment;
                drawCounter++;
            }
        }

        if (drawCounter >= BATCH_DRAWCALLS)
            Render();

        draws[drawCounter - 1]->mode = mode;
        draws[drawCounter - 1]->vertexCount = 0;
        draws[drawCounter - 1]->textureId = defaultTextureId;
    }
}

void RenderBatch::BeginTransform(const Mat4 &transform)
{
    use_matrix = true;
    modelMatrix = transform;
}

void RenderBatch::EndTransform()
{
    use_matrix = false;
}

void RenderBatch::Vertex3f(float x, float y, float z)
{
    float tx = x;
    float ty = y;
    float tz = z;

    if (use_matrix)
    {
        // glm::vec4 vec = modelMatrix * glm::vec4(x, y, z, 1.0f);
        // tx = vec.x;
        // ty = vec.y;
        // tz = vec.z;
    }

    // Transform provided vector if required
    // if (transformRequired)
    // {
    //     x = transform.m0*x + transform.m4*y + transform.m8*z + transform.m12;
    //     y = transform.m1*x + transform.m5*y + transform.m9*z + transform.m13;
    //     tz = transform.m2*x + transform.m6*y + transform.m10*z + transform.m14;
    // }
    //  LogInfo(" vertex %d  buffer %d",vertexCounter,currentBuffer);

    if (vertexCounter > (vertexBuffer[currentBuffer]->elementCount * 4 - 4))
    {
        if ((draws[drawCounter - 1]->mode == LINES) && (draws[drawCounter - 1]->vertexCount % 2 == 0))
        {
            CheckRenderBatchLimit(2 + 1);
        }
        else if ((draws[drawCounter - 1]->mode == TRIANGLES) && (draws[drawCounter - 1]->vertexCount % 3 == 0))
        {
            CheckRenderBatchLimit(3 + 1);
        }
        else if ((draws[drawCounter - 1]->mode == QUAD) && (draws[drawCounter - 1]->vertexCount % 4 == 0))
        {
            CheckRenderBatchLimit(4 + 1);
        }
    }

    vertexBuffer[currentBuffer]->vertices[3 * vertexCounter] = tx;
    vertexBuffer[currentBuffer]->vertices[3 * vertexCounter + 1] = ty;
    vertexBuffer[currentBuffer]->vertices[3 * vertexCounter + 2] = tz;

    vertexBuffer[currentBuffer]->texcoords[2 * vertexCounter] = texcoordx;
    vertexBuffer[currentBuffer]->texcoords[2 * vertexCounter + 1] = texcoordy;

    vertexBuffer[currentBuffer]->colors[4 * vertexCounter] = colorr;
    vertexBuffer[currentBuffer]->colors[4 * vertexCounter + 1] = colorg;
    vertexBuffer[currentBuffer]->colors[4 * vertexCounter + 2] = colorb;
    vertexBuffer[currentBuffer]->colors[4 * vertexCounter + 3] = colora;

    vertexCounter++;
    draws[drawCounter - 1]->vertexCount++;
}

void RenderBatch::Vertex2f(float x, float y)
{
    Vertex3f(x, y, currentDepth);
}

void RenderBatch::TexCoord2f(float x, float y)
{
    texcoordx = x;
    texcoordy = y;
}

void RenderBatch::SetColor(const Color &color)
{
    colorr = color.r;
    colorg = color.g;
    colorb = color.b;
    colora = color.a;
}

unsigned char floatToUnsignedChar(float value)
{
    float normalizedValue = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f
                                                                   : value;
    float scaledValue = normalizedValue * 255.0f;
    scaledValue = (scaledValue < 0) ? 0 : (scaledValue > 255) ? 255
                                                              : scaledValue;
    return (unsigned char)scaledValue;
}

void RenderBatch::SetColor(float r, float g, float b)
{
    colorr = floatToUnsignedChar(r);
    colorg = floatToUnsignedChar(g);
    colorb = floatToUnsignedChar(b);
}

void RenderBatch::SetColor(u8 r, u8 g, u8 b, u8 a)
{
    colorr = r;
    colorg = g;
    colorb = b;
    colora = a;
}

void RenderBatch::SetTexture(unsigned int id)
{
    if (id == 0)
    {
        if (vertexCounter >= vertexBuffer[currentBuffer]->elementCount * 4)
        {
            Render();
        }
    }
    else
    {
        if (draws[drawCounter - 1]->textureId != id)
        {
            if (draws[drawCounter - 1]->vertexCount > 0)
            {
                if (draws[drawCounter - 1]->mode == LINES)
                    draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4) ? draws[drawCounter - 1]->vertexCount : draws[drawCounter - 1]->vertexCount % 4);
                else if (draws[drawCounter - 1]->mode == TRIANGLES)
                    draws[drawCounter - 1]->vertexAlignment = ((draws[drawCounter - 1]->vertexCount < 4) ? 1 : (4 - (draws[drawCounter - 1]->vertexCount % 4)));
                else
                    draws[drawCounter - 1]->vertexAlignment = 0;

                if (!CheckRenderBatchLimit(draws[drawCounter - 1]->vertexAlignment))
                {
                    vertexCounter += draws[drawCounter - 1]->vertexAlignment;
                    drawCounter++;
                }
            }

            if (drawCounter >= BATCH_DRAWCALLS)
                Render();

            draws[drawCounter - 1]->textureId = id;
            draws[drawCounter - 1]->vertexCount = 0;
        }
    }
}



void RenderBatch::DrawCircle(int centerX, int centerY, float radius, const Color &color, bool fill)
{
        SetTexture(0);
    if (fill)
    {
        SetMode(TRIANGLES);
        SetColor(color.r, color.g, color.b, color.a);
        float x = centerX;
        float y = centerY;
        float angle = 0.0f;
        float angleInc = 1.0f / radius;
        for (int i = 0; i < 360; i++)
        {
            Vertex2f(x, y);
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
            angle += angleInc;
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
        }
    }
    else
    {
        SetMode(LINES);
        SetColor(color.r, color.g, color.b, color.a);
        float x = centerX;
        float y = centerY;
        float angle = 0.0f;
        float angleInc = 1.0f / radius;
        for (int i = 0; i < 360; i++)
        {
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
            angle += angleInc;
            Vertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
        }
    }
}

void RenderBatch::DrawRectangle(int posX, int posY, int width, int height, const Color &color, bool fill)
{
    SetTexture(0);
    if (fill)
    {
        SetMode(TRIANGLES);
     

        float x = posX;
        float y = posY;

        SetColor(color.r, color.g, color.b, color.a);
        

        Vertex2f(x   , y);
        Vertex2f(x, y + height);
        Vertex2f( x + width  , y);

        Vertex2f( x + width   , y);
        Vertex2f(x , y + height);
        Vertex2f( x + width, y + height);
    
    } else 
    {
        SetMode(LINES);
        SetColor(color.r, color.g, color.b, color.a);

        Vertex2f(posX , posY );
        Vertex2f(posX + width, posY );

        Vertex2f(posX + width, posY );
        Vertex2f(posX + width, posY + height);

        Vertex2f(posX + width, posY + height);
        Vertex2f(posX , posY + height);

        Vertex2f(posX , posY + height);
        Vertex2f(posX , posY );
    
    }
}

void RenderBatch::DrawRectangle(const Rectangle &rect, const Color &color, bool fill)
{
    DrawRectangle((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, color, fill);
}

void RenderBatch::Line2D(int startPosX, int startPosY, int endPosX, int endPosY)
{
    SetMode(LINES);
    Vertex2f((float)startPosX, (float)startPosY);
    Vertex2f((float)endPosX, (float)endPosY);
}

void RenderBatch::Line2D(const Vec2 &start, const Vec2 &end)
{
    SetMode(LINES);
    Vertex2f(start.x, start.y);
    Vertex2f(end.x, end.y);
}

void RenderBatch::Line3D(const Vec3 &start, const Vec3 &end)
{
    SetMode(LINES);
    Vertex3f(start.x, start.y, start.z);
    Vertex3f(end.x, end.y, end.z);
}

// void RenderBatch::Box(const BoundingBox &box)
// {
//     SetMode(LINES);

//     Line3D(box.min.x, box.min.y, box.min.z, box.max.x, box.min.y, box.min.z);
//     Line3D(box.max.x, box.min.y, box.min.z, box.max.x, box.max.y, box.min.z);
//     Line3D(box.max.x, box.max.y, box.min.z, box.min.x, box.max.y, box.min.z);
//     Line3D(box.min.x, box.max.y, box.min.z, box.min.x, box.min.y, box.min.z);

//     Line3D(box.min.x, box.min.y, box.max.z, box.max.x, box.min.y, box.max.z);
//     Line3D(box.max.x, box.min.y, box.max.z, box.max.x, box.max.y, box.max.z);
//     Line3D(box.max.x, box.max.y, box.max.z, box.min.x, box.max.y, box.max.z);
//     Line3D(box.min.x, box.max.y, box.max.z, box.min.x, box.min.y, box.max.z);

//     Line3D(box.min.x, box.min.y, box.min.z, box.min.x, box.min.y, box.max.z);
//     Line3D(box.max.x, box.min.y, box.min.z, box.max.x, box.min.y, box.max.z);
//     Line3D(box.max.x, box.max.y, box.min.z, box.max.x, box.max.y, box.max.z);
//     Line3D(box.min.x, box.max.y, box.min.z, box.min.x, box.max.y, box.max.z);
// }

// void RenderBatch::Cube(const Vec3 &position, float w, float h, float d, bool wire)
// {
//     float x = position.x;
//     float y = position.y;
//     float z = position.z;

//     if (wire)
//     {
//         SetMode(LINES);
//         Line3D(x, y, z, x + w, y, z);
//         Line3D(x + w, y, z, x + w, y + h, z);
//         Line3D(x + w, y + h, z, x, y + h, z);
//         Line3D(x, y + h, z, x, y, z);

//         Line3D(x, y, z + d, x + w, y, z + d);
//         Line3D(x + w, y, z + d, x + w, y + h, z + d);
//         Line3D(x + w, y + h, z + d, x, y + h, z + d);
//         Line3D(x, y + h, z + d, x, y, z + d);

//         Line3D(x, y, z, x, y, z + d);
//         Line3D(x + w, y, z, x + w, y, z + d);
//         Line3D(x + w, y + h, z, x + w, y + h, z + d);
//         Line3D(x, y + h, z, x, y + h, z + d);
//     }
//     else
//     {
//         SetMode(TRIANGLES);
//         Vertex3f(x - w / 2, y - h / 2, z + d / 2); // Bottom Left
//         Vertex3f(x + w / 2, y - h / 2, z + d / 2); // Bottom Right
//         Vertex3f(x - w / 2, y + h / 2, z + d / 2); // Top Left

//         Vertex3f(x + w / 2, y + h / 2, z + d / 2); // Top Right
//         Vertex3f(x - w / 2, y + h / 2, z + d / 2); // Top Left
//         Vertex3f(x + w / 2, y - h / 2, z + d / 2); // Bottom Right

//         // Back face
//         Vertex3f(x - w / 2, y - h / 2, z - d / 2); // Bottom Left
//         Vertex3f(x - w / 2, y + h / 2, z - d / 2); // Top Left
//         Vertex3f(x + w / 2, y - h / 2, z - d / 2); // Bottom Right

//         Vertex3f(x + w / 2, y + h / 2, z - d / 2); // Top Right
//         Vertex3f(x + w / 2, y - h / 2, z - d / 2); // Bottom Right
//         Vertex3f(x - w / 2, y + h / 2, z - d / 2); // Top Left

//         // Top face
//         Vertex3f(x - w / 2, y + h / 2, z - d / 2); // Top Left
//         Vertex3f(x - w / 2, y + h / 2, z + d / 2); // Bottom Left
//         Vertex3f(x + w / 2, y + h / 2, z + d / 2); // Bottom Right

//         Vertex3f(x + w / 2, y + h / 2, z - d / 2); // Top Right
//         Vertex3f(x - w / 2, y + h / 2, z - d / 2); // Top Left
//         Vertex3f(x + w / 2, y + h / 2, z + d / 2); // Bottom Right

//         // Bottom face
//         Vertex3f(x - w / 2, y - h / 2, z - d / 2); // Top Left
//         Vertex3f(x + w / 2, y - h / 2, z + d / 2); // Bottom Right
//         Vertex3f(x - w / 2, y - h / 2, z + d / 2); // Bottom Left

//         Vertex3f(x + w / 2, y - h / 2, z - d / 2); // Top Right
//         Vertex3f(x + w / 2, y - h / 2, z + d / 2); // Bottom Right
//         Vertex3f(x - w / 2, y - h / 2, z - d / 2); // Top Left

//         // Right face
//         Vertex3f(x + w / 2, y - h / 2, z - d / 2); // Bottom Right
//         Vertex3f(x + w / 2, y + h / 2, z - d / 2); // Top Right
//         Vertex3f(x + w / 2, y + h / 2, z + d / 2); // Top Left

//         Vertex3f(x + w / 2, y - h / 2, z + d / 2); // Bottom Left
//         Vertex3f(x + w / 2, y - h / 2, z - d / 2); // Bottom Right
//         Vertex3f(x + w / 2, y + h / 2, z + d / 2); // Top Left

//         // Left face
//         Vertex3f(x - w / 2, y - h / 2, z - d / 2); // Bottom Right
//         Vertex3f(x - w / 2, y + h / 2, z + d / 2); // Top Left
//         Vertex3f(x - w / 2, y + h / 2, z - d / 2); // Top Right

//         Vertex3f(x - w / 2, y - h / 2, z + d / 2); // Bottom Left
//         Vertex3f(x - w / 2, y + h / 2, z + d / 2); // Top Left
//         Vertex3f(x - w / 2, y - h / 2, z - d / 2); // Bottom Right
//     }
// }

void RenderBatch::Sphere(const Vec3 &position, float radius, int rings, int slices, bool wire)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    if (wire)
    {
        SetMode(LINES);

        for (int i = 0; i < rings; ++i)
        {
            float theta = (float)i * M_PI / rings;
            for (int j = 0; j < slices; ++j)
            {
                float phi1 = (float)j * 2 * M_PI / slices;
                float phi2 = (float)(j + 1) * 2 * M_PI / slices;

                Vec3 v1 = Vec3(
                    x + radius * sin(theta) * cos(phi1),
                    y + radius * sin(theta) * sin(phi1),
                    z + radius * cos(theta));

                Vec3 v2 = Vec3(
                    x + radius * sin(theta) * cos(phi2),
                    y + radius * sin(theta) * sin(phi2),
                    z + radius * cos(theta));

                Line3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
            }
        }

        // Desenhar linhas verticais
        for (int j = 0; j < slices; ++j)
        {
            float phi = (float)j * 2 * M_PI / slices;
            for (int i = (float)0; i < rings; ++i)
            {
                float theta1 = i * M_PI / rings;
                float theta2 = (i + 1) * M_PI / rings;

                Vec3 v1 = Vec3(
                    x + radius * sin(theta1) * cos(phi),
                    y + radius * sin(theta1) * sin(phi),
                    z + radius * cos(theta1));

                Vec3 v2 = Vec3(
                    x + radius * sin(theta2) * cos(phi),
                    y + radius * sin(theta2) * sin(phi),
                    z + radius * cos(theta2));

                Line3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
            }
        }
    }
    else
    {

        SetMode(TRIANGLES);
        for (int i = 0; i < rings; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                // Calcular os v�rtices da esfera
                float theta1 = i * M_PI / rings;
                float theta2 = (i + 1) * M_PI / rings;
                float phi1 = j * 2 * M_PI / slices;
                float phi2 = (j + 1) * 2 * M_PI / slices;

                Vec3 v1 = Vec3(
                    x + radius * sin(theta1) * cos(phi1),
                    y + radius * sin(theta1) * sin(phi1),
                    z + radius * cos(theta1));

                Vec3 v2 = Vec3(
                    x + radius * sin(theta1) * cos(phi2),
                    y + radius * sin(theta1) * sin(phi2),
                    z + radius * cos(theta1));

                Vec3 v3 = Vec3(
                    x + radius * sin(theta2) * cos(phi1),
                    y + radius * sin(theta2) * sin(phi1),
                    z + radius * cos(theta2));

                Vec3 v4 = Vec3(
                    x + radius * sin(theta2) * cos(phi2),
                    y + radius * sin(theta2) * sin(phi2),
                    z + radius * cos(theta2));

                // Desenhar os tri�ngulos da esfera
                Vertex3f(v1.x, v1.y, v1.z);
                Vertex3f(v2.x, v2.y, v2.z);
                Vertex3f(v3.x, v3.y, v3.z);

                Vertex3f(v2.x, v2.y, v2.z);
                Vertex3f(v4.x, v4.y, v4.z);
                Vertex3f(v3.x, v3.y, v3.z);
            }
        }
    }
}

void RenderBatch::Cone(const Vec3 &position, float radius, float height, int segments, bool wire)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    if (wire)
    {
        SetMode(LINES);

        // Desenhar linhas que formam a base do cone
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 v1 = Vec3(
                x + radius * cos(theta1),
                y,
                z + radius * sin(theta1));

            Vec3 v2 = Vec3(
                x + radius * cos(theta2),
                y,
                z + radius * sin(theta2));

            Line3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
        }

        // Desenhar linhas que conectam a base ao v�rtice do cone
        Vec3 vertex = Vec3(x, y + height, z);
        for (int i = 0; i < segments; ++i)
        {
            float theta = i * 2 * M_PI / segments;

            Vec3 base = Vec3(
                x + radius * cos(theta),
                y,
                z + radius * sin(theta));

            Line3D(base.x, base.y, base.z, vertex.x, vertex.y, vertex.z);
        }
    }
    else
    {
        SetMode(TRIANGLES);
        Vec3 vertex = Vec3(x, y + height, z);

        // Desenhar tri�ngulos para formar as faces do cone
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 base1 = Vec3(
                x + radius * cos(theta1),
                y,
                z + radius * sin(theta1));

            Vec3 base2 = Vec3(
                x + radius * cos(theta2),
                y,
                z + radius * sin(theta2));

            // Tri�ngulo formado pela base do cone e o v�rtice
            Vertex3f(base1.x, base1.y, base1.z);
            Vertex3f(base2.x, base2.y, base2.z);
            Vertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
}

void RenderBatch::Cylinder(const Vec3 &position, float radius, float height, int segments, bool wire)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    if (wire)
    {
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 base1 = Vec3(
                x + radius * cos(theta1),
                y,
                z + radius * sin(theta1));

            Vec3 base2 = Vec3(
                x + radius * cos(theta2),
                y,
                z + radius * sin(theta2));

            Line3D(base1.x, base1.y, base1.z, base2.x, base2.y, base2.z);
        }

        // Desenhar linhas que formam a base superior do cilindro
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 top1 = Vec3(
                x + radius * cos(theta1),
                y + height,
                z + radius * sin(theta1));

            Vec3 top2 = Vec3(
                x + radius * cos(theta2),
                y + height,
                z + radius * sin(theta2));

            Line3D(top1.x, top1.y, top1.z, top2.x, top2.y, top2.z);
        }

        // Desenhar linhas que conectam a base inferior � superior
        for (int i = 0; i < segments; ++i)
        {
            float theta = i * 2 * M_PI / segments;

            Vec3 base = Vec3(
                x + radius * cos(theta),
                y,
                z + radius * sin(theta));

            Vec3 top = Vec3(
                x + radius * cos(theta),
                y + height,
                z + radius * sin(theta));

            Line3D(base.x, base.y, base.z, top.x, top.y, top.z);
        }
    }
    else
    {
        SetMode(TRIANGLES);

        // Desenhar a base inferior do cilindro
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 base1 = Vec3(
                x + radius * cos(theta1),
                y,
                z + radius * sin(theta1));

            Vec3 base2 = Vec3(
                x + radius * cos(theta2),
                y,
                z + radius * sin(theta2));

            Vertex3f(x, y, z); // Centro da base inferior
            Vertex3f(base1.x, base1.y, base1.z);
            Vertex3f(base2.x, base2.y, base2.z);
        }

        // Desenhar a base superior do cilindro
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 top1 = Vec3(
                x + radius * cos(theta1),
                y + height,
                z + radius * sin(theta1));

            Vec3 top2 = Vec3(
                x + radius * cos(theta2),
                y + height,
                z + radius * sin(theta2));

            Vertex3f(x, y + height, z); // Centro da base superior
            Vertex3f(top2.x, top2.y, top2.z);
            Vertex3f(top1.x, top1.y, top1.z);
        }

        // Desenhar a superf�cie lateral do cilindro
        for (int i = 0; i < segments; ++i)
        {
            float theta1 = i * 2 * M_PI / segments;
            float theta2 = (i + 1) * 2 * M_PI / segments;

            Vec3 base1 = Vec3(
                x + radius * cos(theta1),
                y,
                z + radius * sin(theta1));

            Vec3 base2 = Vec3(
                x + radius * cos(theta2),
                y,
                z + radius * sin(theta2));

            Vec3 top1 = Vec3(
                x + radius * cos(theta1),
                y + height,
                z + radius * sin(theta1));

            Vec3 top2 = Vec3(
                x + radius * cos(theta2),
                y + height,
                z + radius * sin(theta2));

            // Tri�ngulo lateral inferior
            Vertex3f(base1.x, base1.y, base1.z);
            Vertex3f(base2.x, base2.y, base2.z);
            Vertex3f(top2.x, top2.y, top2.z);

            // Tri�ngulo lateral superior
            Vertex3f(base1.x, base1.y, base1.z);
            Vertex3f(top2.x, top2.y, top2.z);
            Vertex3f(top1.x, top1.y, top1.z);
        }
    }
}

void RenderBatch::Grid(int slices, float spacing, bool axes)
{

    int halfSlices = slices / 2;

    for (int i = -halfSlices; i <= halfSlices; i++)
    {
        if (i == 0)
        {
            SetColor(0.5f, 0.5f, 0.5f);
        }
        else
        {
            SetColor(0.75f, 0.75f, 0.75f);
        }
        Line3D(i * spacing, 0, -halfSlices * spacing, i * spacing, 0, halfSlices * spacing);
        Line3D(-halfSlices * spacing, 0, i * spacing, halfSlices * spacing, 0, i * spacing);
    }
    if (axes)
    {
        SetColor(255, 0, 0);
        Line3D(0.0f, 0.5f, 0.0f, 1, 0.5f, 0);
        SetColor(0, 255, 0);
        Line3D(0.0f, 0.5f, 0.0f, 0, 1.5f, 0);
        SetColor(0, 0, 255);
        Line3D(0.0f, 0.5f, 0.0f, 0, 0.5f, 1);
    }
}

// void RenderBatch::Box(const BoundingBox &box, const Mat4 &transform)
// {
//     Vec3 edges[8];
//     box.getEdges(edges);

//     for (int i = 0; i < 8; i++)
//     {
//         edges[i] = transform * edges[i];
//     }

//     SetMode(LINES);
//     Line3D(edges[5], edges[1]);
//     Line3D(edges[1], edges[3]);
//     Line3D(edges[3], edges[7]);
//     Line3D(edges[7], edges[5]);
//     Line3D(edges[0], edges[2]);
//     Line3D(edges[2], edges[6]);
//     Line3D(edges[6], edges[4]);
//     Line3D(edges[4], edges[0]);
//     Line3D(edges[1], edges[0]);
//     Line3D(edges[3], edges[2]);
//     Line3D(edges[7], edges[6]);
//     Line3D(edges[5], edges[4]);
// }

void RenderBatch::Quad(const Vec2 *coords, const Vec2 *texcoords)
{
    SetMode(QUAD);

    TexCoord2f(texcoords[0].x, texcoords[0].y);
    Vertex2f(coords[0].x, coords[0].y);

    TexCoord2f(texcoords[1].x, texcoords[1].y);
    Vertex2f(coords[1].x, coords[1].y);

    TexCoord2f(texcoords[2].x, texcoords[2].y);
    Vertex2f(coords[2].x, coords[2].y);

    TexCoord2f(texcoords[3].x, texcoords[3].y);
    Vertex2f(coords[3].x, coords[3].y);
}

void RenderBatch::Quad(u32 texture, float x, float y, float width, float height)
{

    float left = 0;
    float right = 1;
    float top = 0;
    float bottom = 1;

    SetTexture(texture);

    float x1 = x;
    float y1 = y;
    float x2 = x;
    float y2 = y + height;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x + width;
    float y4 = y;

    Vec2 coords[4];
    Vec2 texcoords[4];

    coords[0].x = x1;
    coords[0].y = y1;
    coords[1].x = x2;
    coords[1].y = y2;
    coords[2].x = x3;
    coords[2].y = y3;
    coords[3].x = x4;
    coords[3].y = y4;

    texcoords[0].x = left;
    texcoords[0].y = top;
    texcoords[1].x = left;
    texcoords[1].y = bottom;
    texcoords[2].x = right;
    texcoords[2].y = bottom;
    texcoords[3].x = right;
    texcoords[3].y = top;

    Quad(coords, texcoords);
}

void RenderBatch::Quad(Texture2D *texture, float x, float y, float width, float height)
{

    float left = 0;
    float right = 1;
    float top = 0;
    float bottom = 1;

    if (texture != nullptr)
    {

        SetTexture(texture->GetID());
    }
    else
    {
        SetTexture(defaultTextureId);
    }

    float x1 = x;
    float y1 = y;
    float x2 = x;
    float y2 = y + height;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x + width;
    float y4 = y;

    Vec2 coords[4];
    Vec2 texcoords[4];

    coords[0].x = x1;
    coords[0].y = y1;
    coords[1].x = x2;
    coords[1].y = y2;
    coords[2].x = x3;
    coords[2].y = y3;
    coords[3].x = x4;
    coords[3].y = y4;

    texcoords[0].x = left;
    texcoords[0].y = top;
    texcoords[1].x = left;
    texcoords[1].y = bottom;
    texcoords[2].x = right;
    texcoords[2].y = bottom;
    texcoords[3].x = right;
    texcoords[3].y = top;

    Quad(coords, texcoords);
}

void RenderBatch::Quad(Texture2D *texture, const FloatRect &src, float x, float y, float width, float height)
{

    float left = 0;
    float right = 1;
    float top = 0;
    float bottom = 1;

    int widthTex = 1;
    int heightTex = 1;

    if (texture != nullptr)
    {
        widthTex = texture->GetWidth();
        heightTex = texture->GetHeight();
        SetTexture(texture->GetID());
    }

    left = (2.0f * src.x + 1.0f) / (2.0f * widthTex);
    right = left + (src.width * 2.0f - 2.0f) / (2.0f * widthTex);
    top = (2.0f * src.y + 1.0f) / (2 * heightTex);
    bottom = top + (src.height * 2.0f - 2.0f) / (2.0f * heightTex);

    float x1 = x;
    float y1 = y;
    float x2 = x;
    float y2 = y + height;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x + width;
    float y4 = y;

    Vec2 coords[4];
    Vec2 texcoords[4];

    coords[0].x = x1;
    coords[0].y = y1;
    coords[1].x = x2;
    coords[1].y = y2;
    coords[2].x = x3;
    coords[2].y = y3;
    coords[3].x = x4;
    coords[3].y = y4;

    texcoords[0].x = left;
    texcoords[0].y = top;
    texcoords[1].x = left;
    texcoords[1].y = bottom;
    texcoords[2].x = right;
    texcoords[2].y = bottom;
    texcoords[3].x = right;
    texcoords[3].y = top;

    Quad(coords, texcoords);
}

//******************************************************************************************************************

Font::Font() 
{
    
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    fontSize = 25;
    spacing = 2;
    maxHeight =1;
    enableClip = false;

    coords[0] = Vec2(0, 0);
    coords[1] = Vec2(0, 0);
    coords[2] = Vec2(0, 0);
    coords[3] = Vec2(0, 0);

    texcoords[0] = Vec2(0, 0);
    texcoords[1] = Vec2(0, 0);
    texcoords[2] = Vec2(0, 0);
    texcoords[3] = Vec2(0, 0);

    clip.x = 0;
    clip.y = 0;
    clip.width = 0;
    clip.height = 0;
    m_baseSize = 0;
    m_glyphCount = 0;
    m_glyphPadding = 0;
    textLineSpacing = 15;
    texture = nullptr;
    batch = nullptr;
}

Font::~Font()
{

}

void Font::Release()
{
     LogInfo("Release Font.");
    if (texture)
    {
        texture->Release();
        delete texture;
        texture = nullptr;
    }
    m_recs.clear();
    m_glyphs.clear();
}

void Font::SetClip(int x, int y, int w, int h)
{
    clip.x = x;
    clip.y = y;
    clip.width = w;
    clip.height = h;
}

void Font::EnableClip(bool enable)
{
    enableClip = enable;
}

int GetCodepoint(const char *text, int *codepointSize)
{

    int codepoint = 0x3f;
    int octet = (unsigned char)(text[0]);
    *codepointSize = 1;

    if (octet <= 0x7f)
    {

        codepoint = text[0];
    }
    else if ((octet & 0xe0) == 0xc0)
    {
        unsigned char octet1 = text[1];

        if ((octet1 == '\0') || ((octet1 >> 6) != 2))
        {
            *codepointSize = 2;
            return codepoint;
        } // Unexpected sequence

        if ((octet >= 0xc2) && (octet <= 0xdf))
        {
            codepoint = ((octet & 0x1f) << 6) | (octet1 & 0x3f);
            *codepointSize = 2;
        }
    }
    else if ((octet & 0xf0) == 0xe0)
    {
        // Three octets
        unsigned char octet1 = text[1];
        unsigned char octet2 = '\0';

        if ((octet1 == '\0') || ((octet1 >> 6) != 2))
        {
            *codepointSize = 2;
            return codepoint;
        } // Unexpected sequence

        octet2 = text[2];

        if ((octet2 == '\0') || ((octet2 >> 6) != 2))
        {
            *codepointSize = 3;
            return codepoint;
        } // Unexpected sequence

        if (((octet == 0xe0) && !((octet1 >= 0xa0) && (octet1 <= 0xbf))) ||
            ((octet == 0xed) && !((octet1 >= 0x80) && (octet1 <= 0x9f))))
        {
            *codepointSize = 2;
            return codepoint;
        }

        if ((octet >= 0xe0) && (octet <= 0xef))
        {
            codepoint = ((octet & 0xf) << 12) | ((octet1 & 0x3f) << 6) | (octet2 & 0x3f);
            *codepointSize = 3;
        }
    }
    else if ((octet & 0xf8) == 0xf0)
    {
        // Four octets
        if (octet > 0xf4)
            return codepoint;

        unsigned char octet1 = text[1];
        unsigned char octet2 = '\0';
        unsigned char octet3 = '\0';

        if ((octet1 == '\0') || ((octet1 >> 6) != 2))
        {
            *codepointSize = 2;
            return codepoint;
        } // Unexpected sequence

        octet2 = text[2];

        if ((octet2 == '\0') || ((octet2 >> 6) != 2))
        {
            *codepointSize = 3;
            return codepoint;
        } // Unexpected sequence

        octet3 = text[3];

        if ((octet3 == '\0') || ((octet3 >> 6) != 2))
        {
            *codepointSize = 4;
            return codepoint;
        } // Unexpected sequence

        if (((octet == 0xf0) && !((octet1 >= 0x90) && (octet1 <= 0xbf))) ||
            ((octet == 0xf4) && !((octet1 >= 0x80) && (octet1 <= 0x8f))))
        {
            *codepointSize = 2;
            return codepoint;
        } // Unexpected sequence

        if (octet >= 0xf0)
        {
            codepoint = ((octet & 0x7) << 18) | ((octet1 & 0x3f) << 12) | ((octet2 & 0x3f) << 6) | (octet3 & 0x3f);
            *codepointSize = 4;
        }
    }

    if (codepoint > 0x10ffff)
        codepoint = 0x3f; // Codepoints after U+10ffff are invalid

    return codepoint;
}

int GetCodepointNext(const char *text, int *codepointSize)
{
    const char *ptr = text;
    int codepoint = 0x3f; // Codepoint (defaults to '?')
    *codepointSize = 1;

    // Get current codepoint and bytes processed
    if (0xf0 == (0xf8 & ptr[0]))
    {
        // 4 byte UTF-8 codepoint
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80) || ((ptr[3] & 0xC0) ^ 0x80))
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x07 & ptr[0]) << 18) | ((0x3f & ptr[1]) << 12) | ((0x3f & ptr[2]) << 6) | (0x3f & ptr[3]);
        *codepointSize = 4;
    }
    else if (0xe0 == (0xf0 & ptr[0]))
    {
        // 3 byte UTF-8 codepoint */
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80))
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x0f & ptr[0]) << 12) | ((0x3f & ptr[1]) << 6) | (0x3f & ptr[2]);
        *codepointSize = 3;
    }
    else if (0xc0 == (0xe0 & ptr[0]))
    {
        // 2 byte UTF-8 codepoint
        if ((ptr[1] & 0xC0) ^ 0x80)
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x1f & ptr[0]) << 6) | (0x3f & ptr[1]);
        *codepointSize = 2;
    }
    else if (0x00 == (0x80 & ptr[0]))
    {
        // 1 byte UTF-8 codepoint
        codepoint = ptr[0];
        *codepointSize = 1;
    }

    return codepoint;
}

float Font::GetHeight()
{
   return maxHeight;
}

Vec2 Font::GetTextSize(const char *text)
{

    Vec2 textSize;

    int size = (int)std::strlen(text); // Get size in bytes of text
    int tempByteCounter = 0;           // Used to count longer text line num chars
    int byteCounter = 0;

    float textWidth = 0.0f;
    float tempTextWidth = 0.0f; // Used to count longer text line width

    float textHeight = (float)m_baseSize;
    float scaleFactor = fontSize / (float)m_baseSize;

    int letter = 0; // Current character
    int index = 0;  // Index position in sprite font

    for (int i = 0; i < size;)
    {
        byteCounter++;

        int next = 0;
        letter = GetCodepointNext(&text[i], &next);
        index = getGlyphIndex(letter);

        i += next;

        if (letter != '\n')
        {
            if (m_glyphs[index].advanceX != 0)
                textWidth += m_glyphs[index].advanceX;
            else
                textWidth += (m_recs[index].width + m_glyphs[index].offsetX);
        }
        else
        {
            if (tempTextWidth < textWidth)
                tempTextWidth = textWidth;
            byteCounter = 0;
            textWidth = 0;
            textHeight += (float)textLineSpacing;
        }

        if (tempByteCounter < byteCounter)
            tempByteCounter = byteCounter;
    }

    if (tempTextWidth < textWidth)
        tempTextWidth = textWidth;

    textSize.x = tempTextWidth * scaleFactor + (float)((tempByteCounter - 1) * spacing);
    textSize.y = textHeight * scaleFactor;

    return textSize;
}

float Font::GetTextWidth(const char *text)
{

    Vec2 textSize;
    int defaultFontSize = 10; // Default Font chars height in pixel
    if (fontSize < defaultFontSize)
        fontSize = defaultFontSize;
    spacing = fontSize / defaultFontSize;
    textSize = GetTextSize(text);
    return textSize.x;
}

void Font::SetSize(float size)
{
    fontSize = size;
}

void Font::SetSpacing(float spacing)
{
    this->spacing = spacing;
}

void Font::SetColor(const Color &color)
{
    this->color = color;
}

void Font::SetColor(float r, float g, float b)
{
    color.r = (u8)(r * 255.0f);
    color.g = (u8)(g * 255.0f);
    color.b = (u8)(b * 255.0f);
}

void Font::drawTextCodepoint(int codepoint, float x, float y)
{
    int index = getGlyphIndex(codepoint);
    float scaleFactor = fontSize / m_baseSize;

    FloatRect srcRec(m_recs[index].x - (float)m_glyphPadding,
                     m_recs[index].y - (float)m_glyphPadding,
                     m_recs[index].width + 2.0f * m_glyphPadding,
                     m_recs[index].height + 2.0f * m_glyphPadding);

    float px = x + m_glyphs[index].offsetX * scaleFactor - (float)m_glyphPadding * scaleFactor;
    float py = y + m_glyphs[index].offsetY * scaleFactor - (float)m_glyphPadding * scaleFactor;
    float w = (m_recs[index].width + 2.0f * m_glyphPadding) * scaleFactor;
    float h = (m_recs[index].height + 2.0f * m_glyphPadding) * scaleFactor;

    drawTexture(srcRec, px, py, w, h);
}

void Font::drawTextCodepoint(RenderBatch *batch,int codepoint, float x, float y)
{
    int index = getGlyphIndex(codepoint);
    float scaleFactor = fontSize / m_baseSize;

    FloatRect srcRec(m_recs[index].x - (float)m_glyphPadding,
                     m_recs[index].y - (float)m_glyphPadding,
                     m_recs[index].width + 2.0f * m_glyphPadding,
                     m_recs[index].height + 2.0f * m_glyphPadding);

    float px = x + m_glyphs[index].offsetX * scaleFactor - (float)m_glyphPadding * scaleFactor;
    float py = y + m_glyphs[index].offsetY * scaleFactor - (float)m_glyphPadding * scaleFactor;
    float w = (m_recs[index].width + 2.0f * m_glyphPadding) * scaleFactor;
    float h = (m_recs[index].height + 2.0f * m_glyphPadding) * scaleFactor;

    drawTexture(batch,srcRec, px, py, w, h);
}

void Font::DrawText(RenderBatch *batch,const char *text, float x, float y,const Color &c)
{
    SetColor(c);
    DrawText(batch,text,x,y);
}

void Font::DrawText(RenderBatch *batch, float x, float y, const Color &color, const char *text, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    DrawText(batch, buffer, x, y, color);
}

void Font::DrawText(RenderBatch *batch,const char *text, float x, float y)
{

    int size = static_cast<int>(strlen(text));

    int textOffsetY = 0;      // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f; // Offset X to next character to draw

    float scaleFactor = fontSize / m_baseSize; // Character quad scaling factor

    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte std::string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = getGlyphIndex(codepoint);

        if (codepoint == '\n')
        {
            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                drawTextCodepoint(codepoint, x + textOffsetX, y + textOffsetY);
            }

            if (m_glyphs[index].advanceX == 0)
                textOffsetX += ((float)m_recs[index].width * scaleFactor + spacing);
            else
                textOffsetX += ((float)m_glyphs[index].advanceX * scaleFactor + spacing);
        }

        i += codepointByteCount;
    }

    // drawTexture(FloatRect(0,0,100,100),100,100,512,256);
}

void Font::drawTexture(RenderBatch *batch,const FloatRect &src, float x, float y, float width, float height)
{
    if (texture == nullptr)
    {
        LogError("Font texture is not loaded");
        return;
    }
    if (batch == nullptr)
    {
        LogError("RenderBatch is not set");
        return;
    }

    float left = 0;
    float right = 1;
    float top = 0;
    float bottom = 1;

    int widthTex = texture->GetWidth();
    int heightTex = texture->GetHeight();

    batch->SetTexture(texture->GetID());

    left = (2.0f * src.x + 1.0f) / (2.0f * widthTex);
    right = left + (src.width * 2.0f - 2.0f) / (2.0f * widthTex);
    top = (2.0f * src.y + 1.0f) / (2 * heightTex);
    bottom = top + (src.height * 2.0f - 2.0f) / (2.0f * heightTex);

    float x1 = x;
    float y1 = y;
    float x2 = x;
    float y2 = y + height;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x + width;
    float y4 = y;

    float minx_x = Min(Min(x1, x2), Min(x3, x4));
    float minx_y = Min(Min(y1, y2), Min(y3, y4));
    float maxx_x = Max(Max(x1, x2), Max(x3, x4));
    float maxx_y = Max(Max(y1, y2), Max(y3, y4));

    float WIDTH = maxx_x - minx_x;
    float HEIGHT = maxx_y - minx_y;

    coords[0].x = x1;
    coords[0].y = y1;
    coords[1].x = x2;
    coords[1].y = y2;
    coords[2].x = x3;
    coords[2].y = y3;
    coords[3].x = x4;
    coords[3].y = y4;

    texcoords[0].x = left;
    texcoords[0].y = top;
    texcoords[1].x = left;
    texcoords[1].y = bottom;
    texcoords[2].x = right;
    texcoords[2].y = bottom;
    texcoords[3].x = right;
    texcoords[3].y = top;

    float quadLeft = Min(Min(coords[0].x, coords[1].x), Min(coords[2].x, coords[3].x));
    float quadTop = Min(Min(coords[0].y, coords[1].y), Min(coords[2].y, coords[3].y));
    float quadRight = Max(Max(coords[0].x, coords[1].x), Max(coords[2].x, coords[3].x));
    float quadBottom = Max(Max(coords[0].y, coords[1].y), Max(coords[2].y, coords[3].y));

    if (enableClip)
    {
        if (quadRight < clip.x || quadLeft > clip.x + clip.width || quadBottom < clip.y || quadTop > clip.y + clip.height)
        {
            return;
        }

        if (x1 < clip.x)
        {
            float delta = clip.x - x1;
            float ratio = delta / WIDTH;
            coords[0].x = coords[1].x = clip.x;
            texcoords[0].x = texcoords[1].x = left + (right - left) * ratio;
        }

        if (x1 + WIDTH > clip.x + clip.width)
        {
            float delta = (x1 + WIDTH) - (clip.x + clip.width);
            float ratio = delta / WIDTH;
            coords[2].x = coords[3].x = clip.x + clip.width;
            texcoords[2].x = texcoords[3].x = right - (right - left) * ratio;
        }

        if (y1 < clip.y)
        {
            float delta = clip.y - y1;
            float ratio = delta / HEIGHT;
            coords[0].y = coords[3].y = clip.y;
            texcoords[0].y = texcoords[3].y = top + (bottom - top) * ratio;
        }

        if (y1 + HEIGHT > clip.y + clip.height)
        {
            float delta = (y1 + HEIGHT) - (clip.y + clip.height);
            float ratio = delta / HEIGHT;
            coords[1].y = coords[2].y = clip.y + clip.height;
            texcoords[1].y = texcoords[2].y = bottom - (bottom - top) * ratio;
        }
    }

    batch->Quad(coords, texcoords);
}
void Font::drawTexture(const FloatRect &src, float x, float y, float width, float height)
{
    if (texture == nullptr)
    {
        LogError("Font texture is not loaded");
        return;
    }
    if (batch == nullptr)
    {
        LogError("RenderBatch is not set");
        return;
    }

    float left = 0;
    float right = 1;
    float top = 0;
    float bottom = 1;

    int widthTex = texture->GetWidth();
    int heightTex = texture->GetHeight();

    batch->SetTexture(texture->GetID());

    left = (2.0f * src.x + 1.0f) / (2.0f * widthTex);
    right = left + (src.width * 2.0f - 2.0f) / (2.0f * widthTex);
    top = (2.0f * src.y + 1.0f) / (2 * heightTex);
    bottom = top + (src.height * 2.0f - 2.0f) / (2.0f * heightTex);

    float x1 = x;
    float y1 = y;
    float x2 = x;
    float y2 = y + height;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x + width;
    float y4 = y;

    float minx_x = Min(Min(x1, x2), Min(x3, x4));
    float minx_y = Min(Min(y1, y2), Min(y3, y4));
    float maxx_x = Max(Max(x1, x2), Max(x3, x4));
    float maxx_y = Max(Max(y1, y2), Max(y3, y4));

    float WIDTH = maxx_x - minx_x;
    float HEIGHT = maxx_y - minx_y;

    coords[0].x = x1;
    coords[0].y = y1;
    coords[1].x = x2;
    coords[1].y = y2;
    coords[2].x = x3;
    coords[2].y = y3;
    coords[3].x = x4;
    coords[3].y = y4;

    texcoords[0].x = left;
    texcoords[0].y = top;
    texcoords[1].x = left;
    texcoords[1].y = bottom;
    texcoords[2].x = right;
    texcoords[2].y = bottom;
    texcoords[3].x = right;
    texcoords[3].y = top;

    float quadLeft = Min(Min(coords[0].x, coords[1].x), Min(coords[2].x, coords[3].x));
    float quadTop = Min(Min(coords[0].y, coords[1].y), Min(coords[2].y, coords[3].y));
    float quadRight = Max(Max(coords[0].x, coords[1].x), Max(coords[2].x, coords[3].x));
    float quadBottom = Max(Max(coords[0].y, coords[1].y), Max(coords[2].y, coords[3].y));

    if (enableClip)
    {
        if (quadRight < clip.x || quadLeft > clip.x + clip.width || quadBottom < clip.y || quadTop > clip.y + clip.height)
        {
            return;
        }

        if (x1 < clip.x)
        {
            float delta = clip.x - x1;
            float ratio = delta / WIDTH;
            coords[0].x = coords[1].x = clip.x;
            texcoords[0].x = texcoords[1].x = left + (right - left) * ratio;
        }

        if (x1 + WIDTH > clip.x + clip.width)
        {
            float delta = (x1 + WIDTH) - (clip.x + clip.width);
            float ratio = delta / WIDTH;
            coords[2].x = coords[3].x = clip.x + clip.width;
            texcoords[2].x = texcoords[3].x = right - (right - left) * ratio;
        }

        if (y1 < clip.y)
        {
            float delta = clip.y - y1;
            float ratio = delta / HEIGHT;
            coords[0].y = coords[3].y = clip.y;
            texcoords[0].y = texcoords[3].y = top + (bottom - top) * ratio;
        }

        if (y1 + HEIGHT > clip.y + clip.height)
        {
            float delta = (y1 + HEIGHT) - (clip.y + clip.height);
            float ratio = delta / HEIGHT;
            coords[1].y = coords[2].y = clip.y + clip.height;
            texcoords[1].y = texcoords[2].y = bottom - (bottom - top) * ratio;
        }
    }

    batch->Quad(coords, texcoords);
}

void Font::Print(const char *text, float x, float y)
{

    int size = static_cast<int>(strlen(text));

    int textOffsetY = 0;      // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f; // Offset X to next character to draw

    float scaleFactor = fontSize / m_baseSize; // Character quad scaling factor

    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte std::string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = getGlyphIndex(codepoint);

        if (codepoint == '\n')
        {
            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                drawTextCodepoint(codepoint, x + textOffsetX, y + textOffsetY);
            }

            if (m_glyphs[index].advanceX == 0)
                textOffsetX += ((float)m_recs[index].width * scaleFactor + spacing);
            else
                textOffsetX += ((float)m_glyphs[index].advanceX * scaleFactor + spacing);
        }

        i += codepointByteCount;
    }

    // drawTexture(FloatRect(0,0,100,100),100,100,512,256);
}

void Font::Print(float x, float y, const char *text, ...)
{
#define MAX_TEXTFORMAT_BUFFERS 4 // Maximum number of static buffers for text formatting

    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = {0};
    static int index = 0;
    char *currentBuffer = buffers[index];

    std::memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH); // Clear buffer before using

    va_list args;
    va_start(args, text);
    vsprintf(currentBuffer, text, args);
    va_end(args);
    index += 1;
    if (index >= MAX_TEXTFORMAT_BUFFERS)
        index = 0;
    Print(currentBuffer, x, y);
}

 bool Font::Load(const std::string& filePath)
{

      if (!FileExists(filePath.c_str()))
      {
           LogWarning(" File %s not existe, load defaults ",filePath.c_str());
           LoadDefaultFont();
           return false;
      }  

    m_glyphCount = 224;
    

       const char* fileDir  =GetDirectoryPath(filePath.c_str());
      const char* fileName  =GetFileNameWithoutExt(filePath.c_str()); 

      std::string fontTexturePng = std::string(fileDir) + std::string(fileName) + std::string(".png");
      std::string fontTextureTga = std::string(fileDir) + std::string(fileName) + std::string(".tga");
      if (texture)
      {
        texture->Release();
        delete texture;
        texture = nullptr;
      }


      if (FileExists(fontTexturePng.c_str()))
      {

         //  LogWarning(" Load %s texture ",fontTexturePng.c_str());
           texture = new Texture2D(fontTexturePng.c_str());
      } else if (FileExists(fontTextureTga.c_str()))
      {
        //  LogWarning(" Load %s texture ",fontTextureTga.c_str());
          texture = new Texture2D(fontTextureTga.c_str());
      } else 
      {
        LogError("Texture not found: %s%s",fileDir,fileName);
         LoadDefaultFont();
        return false;
      }

      

    
        Reset();
     
       std::ifstream file(filePath);
       float m_height =1;

       std::vector<Character> m_chars;

       if (file.is_open()) 
       {
        std::string line;
        while (std::getline(file, line)) 
        {
            Character data;
            
            std::istringstream iss(line);
           // std::cout<<line<<std::endl;

            data.width= 1;
            data.height= 1;
            data.xoffset= 0;
            data.yoffset= 0;
            data.x=0;
            data.y=0;



            size_t start = line.find('"');
            size_t end = line.find('"', start + 1);
            if (start != std::string::npos && end != std::string::npos) 
            {

                data.id = static_cast<int>(line[start + 1]);
            }

             // Verificamos se a linha tem o número esperado de valores separados por delimitadores
            std::stringstream ss(line.substr(end + 1));
            char delimiter;
            if (ss >> delimiter >> data.x >> delimiter >> data.y >> delimiter >> data.width >> delimiter >> data.height >> delimiter) 
            {
            
                if (delimiter != ',' || ss.fail() || data.width <= 1 || data.height <= 1) 
                {
                    LogError("wrong format: %s", line.c_str());
                    LoadDefaultFont();
          
                   return false;
                }

                m_chars.push_back(data);


                if (data.height > m_height) 
                {
                    m_height = data.height;
                }


            }

         
          
        }
        file.close();


        m_glyphCount =(int) m_chars.size();   
        m_glyphPadding = 0;   



        m_glyphs.reserve(m_glyphCount);
        m_recs.reserve(m_glyphCount);

    
    

   

    texture ->SetMinFilter(FilterMode::Linear);
    texture ->SetMagFilter(FilterMode::Linear);
    texture ->SetWrapS(WrapMode::ClampToEdge);
    texture ->SetWrapT(WrapMode::ClampToEdge);
 




    for (int i = 0; i < m_glyphCount; i++)
    {

        Character c = m_chars[i];
  

        m_glyphs[i].value = c.id;

        m_recs[i].x = c.x;
        m_recs[i].y = c.y;
        m_recs[i].width  = c.width;
        m_recs[i].height = c.height;

        m_glyphs[i].offsetX = c.xoffset;
        m_glyphs[i].offsetY = c.yoffset;
        m_glyphs[i].advanceX = 0;
    }

 

     m_baseSize = (int)m_recs[0].height;

    




        return true;
    } else 
    {
         LoadDefaultFont();
         LogError("File not found: %s", filePath);
        return false;
    }
 return false;

}

int Font::getGlyphIndex(int codepoint)
{

    if (m_glyphs.size()<9)
    {
            return 0;
    }
    int index = 0;
    int fallbackIndex = 0; // Get index of fallback glyph '?'

    // Look for character index in the unordered charset
    for (int i = 0; i < m_glyphCount; i++)
    {
        if (m_glyphs[i].value == 63)
            fallbackIndex = i;

        if (m_glyphs[i].value == codepoint)
        {
            index = i;
            break;
        }
    }

    if ((index == 0) && (m_glyphs[0].value != codepoint))
        index = fallbackIndex;

    return index;
}

#include "data.cc"


bool Font::LoadDefaultFont()
{

#define BIT_CHECK(a, b) ((a) & (1u << (b)))

    m_glyphCount = 224;
    m_glyphPadding = 0;

    int charsHeight = 10;
    int charsDivisor = 1; // Every char is separated from the consecutive by a 1 pixel divisor, horizontally and vertically

    Pixmap pixmap(128, 128, 2);

    for (int i = 0, counter = 0; i < pixmap.width * pixmap.height; i += 32)
    {
        for (int j = 31; j >= 0; j--)
        {

            if (BIT_CHECK(defaultFontData[counter], j))
            {
                ((unsigned short *)pixmap.pixels)[i + j] = 0xffff;
            }
            else
                ((unsigned short *)pixmap.pixels)[i + j] = 0x00ff;
        }
        counter++;
    }

    m_glyphs.resize(m_glyphCount);
    m_recs.resize(m_glyphCount);

    if (texture)
    {
        texture->Release();
        delete texture;
    
    }

    texture = new Texture2D(pixmap);

    texture->SetMinFilter(FilterMode::Nearest);
    texture->SetMagFilter(FilterMode::Nearest);
    texture->SetWrapS(WrapMode::Repeat);
    texture->SetWrapT(WrapMode::Repeat);

    int currentLine = 0;
    int currentPosX = charsDivisor;
    int testPosX = charsDivisor;

    for (int i = 0; i < m_glyphCount; i++)
    {
        m_glyphs[i].value = 32 + i; // First char is 32

        m_recs[i].x = (int)currentPosX;
        m_recs[i].y = (int)(charsDivisor + currentLine * (charsHeight + charsDivisor));
        m_recs[i].width = (int)charsWidth[i];
        m_recs[i].height = (int)charsHeight;
        if (m_recs[i].height > maxHeight)
        {
            maxHeight =m_recs[i].height;
        }

        testPosX += (int)(m_recs[i].width + (float)charsDivisor);

        if (testPosX >= texture->GetWidth())
        {
            currentLine++;
            currentPosX = 2 * charsDivisor + charsWidth[i];
            testPosX = currentPosX;

            m_recs[i].x = (int)charsDivisor;
            m_recs[i].y = (int)(charsDivisor + currentLine * (charsHeight + charsDivisor));
        }
        else
            currentPosX = testPosX;

        // NOTE: On default defaultFont character offsets and xAdvance are not required
        m_glyphs[i].offsetX = 0;
        m_glyphs[i].offsetY = 0;
        m_glyphs[i].advanceX = 0;
    }

    // pixmap.Save("font.png");

    m_baseSize = (int)m_recs[0].height;

    LogInfo("[FONT]: Default font loaded successfully (%i glyphs)", m_glyphCount);

    return true;
}



bool Font::Reset()
{


    m_glyphCount = 224;
    m_glyphPadding = 0;

    int charsHeight = 10;
    int charsDivisor = 1; // Every char is separated from the consecutive by a 1 pixel divisor, horizontally and vertically

    m_glyphs.resize(m_glyphCount);
    m_recs.resize(m_glyphCount);

  

    int currentLine = 0;
    int currentPosX = charsDivisor;
    int testPosX = charsDivisor;

    for (int i = 0; i < m_glyphCount; i++)
    {
        m_glyphs[i].value = 32 + i; // First char is 32

        m_recs[i].x = 0;
        m_recs[i].y = 0;
        m_recs[i].width = 16;
        m_recs[i].height = 16;
      
     

        // NOTE: On default defaultFont character offsets and xAdvance are not required
        m_glyphs[i].offsetX = 0;
        m_glyphs[i].offsetY = 0;
        m_glyphs[i].advanceX = 0;
    }

    // pixmap.Save("font.png");

    m_baseSize = (int)m_recs[0].height;


    return true;
}