///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
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

#ifndef DEVICES_SOAPYSDR_DEVICESOAPYSDRPARAMS_H_
#define DEVICES_SOAPYSDR_DEVICESOAPYSDRPARAMS_H_

#include <stdint.h>
#include <SoapySDR/Device.hpp>

#include "export.h"

/**
 * This structure refers to one physical device shared among parties (logical devices represented by
 * the DeviceSinkAPI or DeviceSourceAPI).
 * It allows storing information on the common resources in one place and is shared among participants.
 * There is only one copy that is constructed by the first participant and destroyed by the last.
 * A participant knows it is the first or last by checking the lists of buddies (Rx + Tx).
 */

class DEVICES_API DeviceSoapySDRParams
{
public:
    struct GainSetting
    {
        std::string m_name;           //!< Name of the gain element
        SoapySDR::Range m_range;      //!< Gain range
    };

    struct FrequencySetting
    {
        std::string m_name;           //!< Name of the tunable element
        SoapySDR::RangeList m_ranges; //!< List of ranges of the tunable element
    };

    struct ChannelSetting
    {
        SoapySDR::ArgInfoList m_streamSettingsArgs;        //!< common stream parameters
        bool m_hasDCAutoCorrection;                        //!< DC offset auto correction flag
        bool m_hasDCOffsetValue;                           //!< DC offset value flag
        bool m_hasIQBalanceValue;                          //!< IQ correction value flag
        bool m_hasFrequencyCorrectionValue;                //!< Frequency correction value flag
        std::vector<std::string> m_antennas;               //!< Antenna ports names
        bool m_hasAGC;                                     //!< AGC flag
        SoapySDR::Range m_gainRange;                       //!< Global gain range
        std::vector<GainSetting> m_gainSettings;           //!< gain elements settings
        std::vector<FrequencySetting> m_frequencySettings; //!< tunable elements settings
        SoapySDR::ArgInfoList m_frequencySettingsArgs;     //!< common tuning parameters
        SoapySDR::RangeList m_ratesRanges;                 //!< list of ranges of sample rates
        SoapySDR::RangeList m_bandwidthsRanges;            //!< list of ranges of bandwidths
    };

    DeviceSoapySDRParams(SoapySDR::Device *device);
    ~DeviceSoapySDRParams();

private:
    void fillParams();
    void fillChannelParams(std::vector<ChannelSetting>& channelSettings, int direction, unsigned int ichan);
    SoapySDR::Device *m_device;
    SoapySDR::ArgInfoList m_deviceSettingsArgs; //!< list (vector) of device settings arguments
    uint32_t m_nbRx; //!< number of Rx channels
    uint32_t m_nbTx; //!< number of Tx channels
    std::vector<ChannelSetting> m_RxChannelsSettings;
    std::vector<ChannelSetting> m_TxChannelsSettings;
};


#endif /* DEVICES_SOAPYSDR_DEVICESOAPYSDRPARAMS_H_ */
