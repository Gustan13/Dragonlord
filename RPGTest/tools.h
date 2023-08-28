#pragma once
#ifndef _HELLO_
#define _HELLO_

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

#endif // !1
