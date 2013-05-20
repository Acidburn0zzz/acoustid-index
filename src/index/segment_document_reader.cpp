// Copyright (C) 2013  Lukas Lalinsky
// Distributed under the MIT license, see the LICENSE file for details.

#include "store/input_stream.h"
#include "segment_document_reader.h"

using namespace Acoustid;

SegmentDocumentReader::SegmentDocumentReader(InputStream *input)
	: m_input(input)
{
}

SegmentDocumentReader::~SegmentDocumentReader()
{
}

void SegmentDocumentReader::readDocument(size_t position, uint32_t **dataOut, size_t *lengthOut)
{
	m_input->seek(position);

	size_t length = m_input->readVInt32();
	uint32_t *data = new uint32_t[length];

	for (size_t i = 0; i < length; i++) {
		data[i] = m_input->readInt32();
	}

	*dataOut = data;
	*lengthOut = length;
}

SegmentDocumentIndexSharedPtr SegmentDocumentReader::readIndex(InputStream *input, size_t documentCount)
{
	SegmentDocumentIndexSharedPtr index(new SegmentDocumentIndex(documentCount));

	uint32_t *ids = index->ids();
	size_t *positions = index->positions();

	for (size_t i = 0; i < documentCount; i++) {
		*ids++ = input->readVInt32();
		*positions++ = input->readVInt64();
	}

	return index;
}
