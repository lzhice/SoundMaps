#include "qportaudiomanager.h"

#include "config.h"

#include <qshareddata.h>
#include <qqml.h>
class QPortAudioManagerPrivate : QSharedData {
    Q_DISABLE_COPY(QPortAudioManagerPrivate)
    Q_DECLARE_PUBLIC(QPortAudioManager)
public:
    QPortAudioManagerPrivate(QPortAudioManager* parent) :
        q_ptr(parent),
        audioSeries(new QPortAudioSeries(parent)),
        recorder(new QPortAudioRecorder(parent))
    {

    }

    void setRecorder(QPortAudioRecorder* tmp) {
        Q_Q(QPortAudioManager);
        if (tmp != recorder) {
            recorder = tmp;
            emit q->recorderChanged(recorder);
        }
    }

    void setAudioSeries(QPortAudioSeries* tmp) {
        Q_Q(QPortAudioManager);
        if (tmp != audioSeries) {
            audioSeries = tmp;
            emit q->audioSeriesChanged(audioSeries);
        }
    }

    QPortAudioManager* const q_ptr;
    QPortAudioSeries* audioSeries;
    QPortAudioRecorder *recorder;
};

QPortAudioManager::QPortAudioManager(QObject *parent) :
    QObject(parent),
    d_ptr(new QPortAudioManagerPrivate(this))
{
    qmlRegisterType<QPortAudioRecorder>(PACKAGE_NAME, PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, "QPortAudioRecorder");
    qmlRegisterType<QPortAudioSeries>(PACKAGE_NAME, PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, "QPortAudioSeries");

    Q_D(QPortAudioManager);
    connect(d->recorder, &QPortAudioRecorder::onBufferReady, this, [&](float * _t1, ulong _t2){
        Q_D(QPortAudioManager);
        d->audioSeries->appendBuffer(_t1, _t2);
    });
}

QPortAudioManager::~QPortAudioManager() {
    delete d_ptr;
}

QPortAudioSeries *QPortAudioManager::audioSeries() const {
    Q_D(const QPortAudioManager);
    return d->audioSeries;
}

QPortAudioRecorder *QPortAudioManager::recorder() const {
    Q_D(const QPortAudioManager);
    return d->recorder;
}


void QPortAudioManager::reset() {
    Q_D(QPortAudioManager);
    d->recorder->reset();
    d->audioSeries->initialize(d->recorder->sampleRate(), d->recorder->frameLength());
}

void QPortAudioManager::setAudioSeries(QPortAudioSeries *source) {
    Q_D(QPortAudioManager);
    d->setAudioSeries(source);
}
void QPortAudioManager::setRecorder(QPortAudioRecorder *recorder) {
    Q_D(QPortAudioManager);
    d->setRecorder(recorder);
}