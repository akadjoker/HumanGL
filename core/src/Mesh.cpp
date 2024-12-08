#include "Mesh.hpp"

//***********************************************************************************************************

VertexFormat::VertexFormat(const Element *elements, unsigned int elementCount)
    : _vertexSize(0)
{
    for (unsigned int i = 0; i < elementCount; ++i)
    {
        Element element;
        memcpy(&element, &elements[i], sizeof(Element));
        _elements.push_back(element);
        _vertexSize += element.size * sizeof(float);
    }
}

VertexFormat::~VertexFormat() {}

const VertexFormat::Element &VertexFormat::getElement(unsigned int index) const { return _elements[index]; }

unsigned int VertexFormat::getElementCount() const { return (unsigned int)_elements.size(); }
unsigned int VertexFormat::getVertexSize() const { return _vertexSize; }

bool VertexFormat::operator==(const VertexFormat &f) const
{
    if (_elements.size() != f._elements.size())
        return false;
    for (size_t i = 0, count = _elements.size(); i < count; ++i)
    {
        if (_elements[i] != f._elements[i])
            return false;
    }

    return true;
}
bool VertexFormat::operator!=(const VertexFormat &f) const { return !(*this == f); }
VertexFormat::Element::Element() : usage(VertexType::POSITION), size(0) {}

VertexFormat::Element::Element(VertexType usage, unsigned int size) : usage(usage), size(size) {}
bool VertexFormat::Element::operator==(const VertexFormat::Element &e) const { return (size == e.size && usage == e.usage); }
bool VertexFormat::Element::operator!=(const VertexFormat::Element &e) const { return !(*this == e); }



MeshBuffer::MeshBuffer(bool dynamic)
{
    m_vbo = 0;
    m_ibo = 0;
    m_vao = 0;
    m_dynamic = dynamic;
    m_useIndices = false;
}

MeshBuffer::~MeshBuffer()
{

    Release();
}

void MeshBuffer::CreateVertexBuffer(const VertexFormat &vertexFormat, unsigned int vertexCount)
{
    m_vertexFormat = vertexFormat;
    m_numVertices = vertexCount;
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * vertexCount, NULL, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    if (!m_vao)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
    }
    else
    {
        glBindVertexArray(m_vao);
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    unsigned int offset = 0;
    for (unsigned int i = 0; i < vertexFormat.getElementCount(); ++i)
    {
        const VertexFormat::Element &element = vertexFormat.getElement(i);

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (GLint)element.size, GL_FLOAT, GL_FALSE, (GLsizei)vertexFormat.getVertexSize(), (void *)(offset * sizeof(float)));

        offset += element.size;
    }

    glBindVertexArray(0);
}

void MeshBuffer::CreateIndexBuffer(unsigned int indexCount)
{
    if (!m_vao)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
    }
    else
    {
        glBindVertexArray(m_vao);
    }
    m_useIndices = true;
    m_numIndices = indexCount;
    glGenBuffers(1, &this->m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), NULL, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void MeshBuffer::SetIndexData(const void *indexData)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_numIndices * sizeof(unsigned int), indexData);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshBuffer::SetVertexData(const void *vertexData)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * m_vertexFormat.getVertexSize(), vertexData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::Render(int mode, int count)
{

    glBindVertexArray(m_vao);
    if (m_useIndices)
    {

        glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    }
    else
    {

        glDrawArrays(mode, 0, count);
    }
}

void MeshBuffer::Release()
{

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_useIndices && m_ibo != 0)
    {
        glDeleteBuffers(1, &m_ibo);
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
    }

    m_vbo = 0;
    m_ibo = 0;
}