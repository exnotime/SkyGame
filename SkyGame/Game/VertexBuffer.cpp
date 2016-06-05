#include "VertexBuffer.h"

VertexBuffer::VertexBuffer ( void ) {
	m_Handle = 0;
	m_VertexArrayId = 0;
}

VertexBuffer::~VertexBuffer( void ) {
	glDeleteBuffers ( 1, &m_Handle );
}

bool VertexBuffer::Init( VertexType VertexType, void* Data, unsigned int Size ) {
	if (m_VertexArrayId == 0)
		glGenVertexArrays ( 1, &m_VertexArrayId );
	if (m_Handle == 0)
		glGenBuffers ( 1, &m_Handle );
	if ( m_Handle == GL_INVALID_VALUE ) {
		return false;
	}

	glBindVertexArray(m_VertexArrayId);
	glBindBuffer ( GL_ARRAY_BUFFER, m_Handle );
	glBufferData ( GL_ARRAY_BUFFER, Size, Data, GL_DYNAMIC_DRAW );
	if ( VertexType == POS_NORMAL_TEX ) {
		glEnableVertexAttribArray ( 0 );
		glEnableVertexAttribArray ( 1 );
		glEnableVertexAttribArray ( 2 );
		glBindBuffer ( GL_ARRAY_BUFFER, m_Handle );
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( VertexPosNormalTex ), 0 );
		glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( VertexPosNormalTex ), ( unsigned char* ) NULL + ( 3 * sizeof ( float ) ) );
		glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( VertexPosNormalTex ), ( unsigned char* ) NULL + ( 6 * sizeof ( float ) ) );
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return glIsBuffer ( m_Handle ) == GL_TRUE;
}


void VertexBuffer::Apply() {
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTex), (unsigned char*)NULL + (3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTex), (unsigned char*)NULL + (6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray( m_VertexArrayId );
}

void VertexBuffer::Release() {
	glDeleteBuffers ( 1, &m_Handle );
}

void VertexBuffer::Update(void* data, unsigned int size, unsigned int offset) {
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
