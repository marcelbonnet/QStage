#ifndef KQUEUE_H
#define KQUEUE_H

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QString>

class Kqueue
{
public:
    Kqueue();
    static bool watch(QString path /*events*/);
};

#endif // KQUEUE_H
