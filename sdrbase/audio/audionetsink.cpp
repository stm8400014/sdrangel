///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 F4EXB                                                      //
// written by Edouard Griffiths                                                  //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include "audionetsink.h"
#include "util/rtpsink.h"

#include <unistd.h>
#include <QUdpSocket>

const int AudioNetSink::m_udpBlockSize = 512;

AudioNetSink::AudioNetSink(QObject *parent, bool stereo) :
    m_type(SinkUDP),
    m_rtpBufferAudio(0),
    m_bufferIndex(0),
    m_port(9998)
{
    m_udpSocket = new QUdpSocket(parent);
    m_rtpBufferAudio = new RTPSink(m_udpSocket, stereo);
}

AudioNetSink::~AudioNetSink()
{
    if (m_rtpBufferAudio) {
        delete m_rtpBufferAudio;
    }

    m_udpSocket->deleteLater(); // this thread is not the owner thread (was moved)
}

bool AudioNetSink::isRTPCapable() const
{
        return m_rtpBufferAudio->isValid();
}

bool AudioNetSink::selectType(SinkType type)
{
    if (type == SinkUDP)
    {
        m_type = SinkUDP;
        return true;
    }
    else if (type == SinkRTP)
    {
        m_type = SinkRTP;
        return true;
    }
    else
    {
        return false;
    }
}

void AudioNetSink::setDestination(const QString& address, uint16_t port)
{
    m_address.setAddress(const_cast<QString&>(address));
    m_port = port;

    if (m_rtpBufferAudio) {
        m_rtpBufferAudio->setDestination(address, port);
    }
}

void AudioNetSink::addDestination(const QString& address, uint16_t port)
{
    if (m_rtpBufferAudio) {
        m_rtpBufferAudio->addDestination(address, port);
    }
}

void AudioNetSink::deleteDestination(const QString& address, uint16_t port)
{
    if (m_rtpBufferAudio) {
        m_rtpBufferAudio->deleteDestination(address, port);
    }
}

void AudioNetSink::write(qint16 sample)
{
    if (m_type == SinkUDP)
    {
        if (m_bufferIndex >= m_udpBlockSize)
        {
            m_udpSocket->writeDatagram((const char*)m_data, (qint64 ) m_udpBlockSize, m_address, m_port);
            m_bufferIndex = 0;
        }
        else
        {
            qint16 *p = (qint16*) &m_data[m_bufferIndex];
            *p = sample;
            m_bufferIndex += sizeof(qint16);
        }
    }
    else if (m_type == SinkRTP)
    {
        m_rtpBufferAudio->write((uint8_t *) &sample);
    }
}

void AudioNetSink::moveToThread(QThread *thread)
{
    m_udpSocket->moveToThread(thread);
}

