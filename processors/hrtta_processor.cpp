#include "hrtta_processor.h"

#include <QVector>
#include <QDebug>

#include <cmath>

THrttaProcessor::THrttaProcessor(QObject *parent)
    : QObject{parent} {
    Status = 0;                  //The function return value or error code. Status == 0 indicates no error. For details please check the Appendix 1 in the API Guide document.
    Device = NULL;              //Device handle. Use the device handle to specify device for manipulating in the API calls. The device handle must be initialized firstly by function Devcie_Open before it to be used.

    m_ReconnectTimer = new QTimer;
    m_ReconnectTimer->setInterval(1000);
    connect(m_ReconnectTimer, SIGNAL(timeout()), SLOT(onReconnectTimer()));
    m_ReconnectTimer->start();

    m_WorkTimer = new QTimer;
    m_WorkTimer->setInterval(100);
    connect(m_WorkTimer, SIGNAL(timeout()), SLOT(onWorkTimer()));
    m_WorkTimer->start();

    m_elapsedTime.start();
}

double THrttaProcessor::centerFreq() {
    return SWP_ProfileOut.CenterFreq_Hz;
}

double THrttaProcessor::level() {
    return SWP_ProfileOut.RefLevel_dBm;
}

double THrttaProcessor::span() {
    return SWP_ProfileOut.Span_Hz;
}

double THrttaProcessor::rbw() {
    return SWP_ProfileOut.RBW_Hz;
}

double THrttaProcessor::vbw() {
    return SWP_ProfileOut.VBW_Hz;
}

uint32_t THrttaProcessor::sweepCount() {
    return TraceInfo.FullsweepTracePoints;
}

QVector<float> THrttaProcessor::getMinSweep() {
    QVector<float> result;

    for(int i = 0; i < TraceInfo.FullsweepTracePoints; ++i) {
        result.append(PowerSpec_dBm[i]);
    }

    return result;
}

uint8_t THrttaProcessor::pickSearchType() {
    return m_pickSearchType;
}

double THrttaProcessor::pickSearchCenter() {
    return m_pickSearchCenter;
}

double THrttaProcessor::pickSearchWidth() {
    return m_pickSearchWidth;
}

double THrttaProcessor::pick() {
    if(m_pickSearchType == 0x01) {
        return getSimpleMaximum();
    }

    if(m_pickSearchType == 0x00) {
        return getIntegralMaximum();
    }

    return -300.0;
}

double THrttaProcessor::perTry() {
    return m_perTry;
}

void THrttaProcessor::onOpenCmd() {

}

void THrttaProcessor::onOpenSerialCmd(uint32_t serial) {

    Q_UNUSED(serial);
}

void THrttaProcessor::onCloseCmd() {

}

void THrttaProcessor::onSetCenter(uint64_t center) {

    m_centerFreq = center;
    m_connected = reconfigure();
}

void THrttaProcessor::onSetLevel(uint64_t level) {

    m_level = level;
    m_connected = reconfigure();
}

void THrttaProcessor::onSetSpan(uint64_t span) {

    m_span = span;
    m_connected = reconfigure();
}

void THrttaProcessor::onSetRbw(uint64_t rbw) {

    m_rbw = rbw;
    m_connected = reconfigure();
}

void THrttaProcessor::onSetVbw(uint64_t vbw) {

    m_vbw = vbw;
    m_connected = reconfigure();
}

void THrttaProcessor::onSetPickSearchType(uint8_t type) {

    m_pickSearchType = type;
}

void THrttaProcessor::onSetPickSearchCenter(uint64_t center) {

    m_pickSearchCenter = center;
}

void THrttaProcessor::onSetPickSearchWidth(uint64_t width) {

    m_pickSearchWidth = width;
}

void THrttaProcessor::onSetPickSearchFullSpan() {

    m_pickSearchCenter = m_centerFreq;
    m_pickSearchWidth = m_span;
}

void THrttaProcessor::onReconnectTimer() {
    if(!m_connected) {
        if(Device != NULL) {
            Device_Close(&Device);
        }

        m_connected = false;

        BootProfile_TypeDef BootProfile; //Parameters for device boot.
        BootInfo_TypeDef BootInfo;       //Feedback information of the devic boot. Hardware version, firmware version and other information.

        BootProfile.DevicePowerSupply = USBPortAndPowerPort; //Both the USB data port and independent power port are used for power supply.
        BootProfile.PhysicalInterface = USB;				 //Usb interface for data transfer.

        int DevNum = 0;
        Status = Device_Open(&Device, DevNum, &BootProfile, &BootInfo); //open the device.

        if(Status == APIRETVAL_NoError) {
            printf("Device is opened successfully\n");
        }
        /*if failed, an error code is returned.Please re-open the device according to suggestions*/
        else {
            switch(Status) {
                case APIRETVAL_ERROR_BusOpenFailed:
                    printf("Error - Check the device power supply, data cable connection and driver installation\n");
                    break;

                case APIRETVAL_ERROR_RFACalFileIsMissing:
                    printf("Error - RF calibration file is missing\n");
                    break;

                case APIRETVAL_ERROR_IFACalFileIsMissing:
                    printf("Error - IF calibration file is missing\n");
                    break;

                case APIRETVAL_ERROR_DeviceConfigFileIsMissing:
                    printf("Error - Configuration file missing\n");
                    break;

                case APIRETVAL_ERROR_DeviceSpecFileIsMissing:
                    printf("Error - Device specification file is missing\n");
                    break;

                default:
                    printf("Return other errors Status = %d\n", Status);
                    break;
            }

            qDebug() << Status;
        }


        Status = Device_QueryDeviceInfo(&Device, &DeviceInfo); //obtain device information including device UID,model, firmware etc.
        qDebug() << "--------------DeviceUID       : " << DeviceInfo.DeviceUID;
        qDebug() << "--------------Model           : " << DeviceInfo.Model;
        qDebug() << "--------------HardwareVersion : " << DeviceInfo.HardwareVersion;
        qDebug() << "--------------MFWVersion      : " << DeviceInfo.MFWVersion;
        qDebug() << "--------------FFWVersion      : " << DeviceInfo.FFWVersion;

        m_centerFreq = 3e9;
        m_span = 0.25e9;
        m_rbw = 100e3;
        m_vbw = 100e3;
        m_level = 0;
        m_connected = reconfigure();
    }
}

void THrttaProcessor::onWorkTimer() {
    if(!m_connected) {
        if(Device != NULL) {
            m_centerFreq = 3e9;
            m_span = 0.25e9;
            m_rbw = 100e3;
            m_vbw = 100e3;
            m_level = 0;
        }

        return;
    }

    if(m_connected) {
        if(m_elapsedTime.elapsed() >= 20000) {
            m_centerFreq = 3e9;
            m_span = 0.25e9;
            m_rbw = 100e3;
            m_vbw = 100e3;
            m_level = 0;
            m_connected = false;
            return;
        }
    }

    m_WorkTimer->stop();

    QElapsedTimer m_elapsed;
    m_elapsed.start();
    Status = SWP_GetFullSweep(&Device, Frequency.data(), PowerSpec_dBm.data(), &MeasAuxInfo);

    if(Status == APIRETVAL_NoError) {
        m_allEllapsed += m_elapsed.elapsed();
        m_perTry = m_elapsed.elapsed();
        //qDebug() << "time     --- " << m_allEllapsed;
        //qDebug() << "count    --- " << m_count;
        //qDebug() << "per try  --- " << test;
        //qDebug() << "sweep count  " << PowerSpec_dBm.count();
        emit translateDataReady();
    } else {
        m_connected = false;
    }

    m_elapsedTime.restart();
    m_WorkTimer->start();
}

bool THrttaProcessor::reconfigure() {

    SWP_ProfileDeInit(&Device, &SWP_ProfileIn);           //initialize the SWP_ProfileIn.
    SWP_ProfileIn.CenterFreq_Hz = m_centerFreq;
    SWP_ProfileIn.Span_Hz = m_span;
    SWP_ProfileIn.StartFreq_Hz = m_centerFreq - (m_span / 2.0);
    SWP_ProfileIn.StopFreq_Hz =  m_centerFreq + (m_span / 2.0);
    SWP_ProfileIn.RefLevel_dBm = m_level;
    SWP_ProfileIn.RBW_Hz = m_rbw;
    SWP_ProfileIn.VBW_Hz = m_vbw;
    SWP_ProfileIn.VBWMode = VBW_Manual;

    Status = SWP_Configuration(&Device, &SWP_ProfileIn, &SWP_ProfileOut, &TraceInfo); //deliever configuration in SWP mode.

    qDebug() << "--------------FullsweepTracePoints" << TraceInfo.FullsweepTracePoints;
    qDebug() << "--------------PartialsweepTracePoints" << TraceInfo.PartialsweepTracePoints;
    qDebug() << "--------------TotalHops" << TraceInfo.TotalHops;
    qDebug() << "--------------UserStartIndex" << TraceInfo.UserStartIndex;
    qDebug() << "--------------UserStopIndex" << TraceInfo.UserStopIndex;
    qDebug() << "--------------TraceBinBW_Hz" << TraceInfo.TraceBinBW_Hz;
    qDebug() << "--------------StartFreq_Hz" << TraceInfo.StartFreq_Hz;
    qDebug() << "--------------AnalysisBW_Hz" << TraceInfo.AnalysisBW_Hz;
    qDebug() << "--------------TraceDetectRatio" << TraceInfo.TraceDetectRatio;
    qDebug() << "--------------DecimateFactor" << TraceInfo.DecimateFactor;
    qDebug() << "--------------FrameTimeMultiple" << TraceInfo.FrameTimeMultiple;
    qDebug() << "--------------FrameTime" << TraceInfo.FrameTime;
    qDebug() << "--------------EstimateMinSweepTime" << TraceInfo.EstimateMinSweepTime;
    qDebug() << "--------------SamplePoints" << TraceInfo.SamplePoints;
    qDebug() << "--------------GainParameter" << TraceInfo.GainParameter;

    if(Status != 0) {
        return false;
    }

    DeviceState_TypeDef DeviceState;                                  //Device state includs the temperature, the RF state etc.
    Status = Device_QueryDeviceState_Realtime(&Device, &DeviceState); //Obtain device state includs the temperature, the RF state etc.

    Frequency.resize(TraceInfo.FullsweepTracePoints);
    PowerSpec_dBm.resize(TraceInfo.FullsweepTracePoints);

    HopIndex = 0;                                                 //Hop index.
    FrameIndex = 0;                                               //Frame index.
    return true;
}

double THrttaProcessor::getSimpleMaximum() {
    QVector<float> spectrum = PowerSpec_dBm;

    if(spectrum.count() <= 0) {
        return -300.0;
    }

    double index = m_centerFreq - m_span / 2.0;
    double indexWeight = m_span / spectrum.count();

    double indexLeft = m_pickSearchCenter - m_pickSearchWidth / 2.0;
    double indexRight = m_pickSearchCenter + m_pickSearchWidth / 2.0;

    double max = spectrum.value(0, -300.0);

    for(int i = 0; i < spectrum.count(); ++i) {
        if((index >= indexLeft) && (index <= indexRight)) {
            if(max < spectrum[i]) {
                max = spectrum[i];
            }
        }

        index += indexWeight;
    }

    //return pick();
    return max;
}

double THrttaProcessor::getIntegralMaximum() {
    float sum = 0.0;
    uint32_t m_itemCount = 0;

    QVector<float> spectrum = PowerSpec_dBm;

    if(spectrum.count() <= 0) {
        return -300.0;
    }

    double index = m_centerFreq - m_span / 2.0;
    double indexWeight = m_span / spectrum.count();

    double indexLeft = m_pickSearchCenter - m_pickSearchWidth / 2.0;
    double indexRight = m_pickSearchCenter + m_pickSearchWidth / 2.0;

    for(int i = 0; i < spectrum.count(); ++i) {
        if((index >= indexLeft) && (index <= indexRight)) {
            sum += pow(10, spectrum[i] * 0.1);
            m_itemCount++;
        }

        index += indexWeight;
    }

    if(m_itemCount == 0) {
        return spectrum.value(0, -300.0);
    }

    sum /= m_pickSearchWidth;
    float res = 10.0 * log10(sum);
    return res;
}
