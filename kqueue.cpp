#include "kqueue.h"

Kqueue::Kqueue()
{

}

bool Kqueue::watch(QString path){
    struct kevent event;    /* Event we want to monitor */
    struct kevent tevent;   /* Event triggered */
    int kq, fd, ret;

    char *p = path.toLatin1().data();

    fd = open(p, O_RDONLY);
    if (fd == -1)
        err(EXIT_FAILURE, "Failed to open '%s'", p);

    /* Create kqueue. */
    kq = kqueue();
    if (kq == -1)
        err(EXIT_FAILURE, "kqueue() failed");

    /* Initialize kevent structure. */
    EV_SET(&event, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE,
        0, NULL);
    /* Attach event to the kqueue. */
    ret = kevent(kq, &event, 1, NULL, 0, NULL);
    if (ret == -1)
        err(EXIT_FAILURE, "kevent register");
    if (event.flags & EV_ERROR)
        errx(EXIT_FAILURE, "Event error: %s", strerror(event.data));

    printf("Monitorando '%s'\n", p);

    //poderia criar um fd membro static para monitorar por fora no intervalo desejado
    for (;;) {
        /* Sleep until something happens. */
        ret = kevent(kq, NULL, 0, &tevent, 1, NULL);
        if (ret == -1) {
            err(EXIT_FAILURE, "kevent wait");
            return false;
        } else if (ret > 0) {
            printf("Something was written in '%s'\n", p);
            return true;
        }
    }

}
