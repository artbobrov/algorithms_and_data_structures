#!/usr/bin/python

from __future__ import print_function


def strong(func):
    def wrapper(name, message):
        return '<strong>' + func(name, message) + '<\strong>'
    return wrapper


def italic(func):
    def wrapper(name, message):
        return '<i>' + func(name, message) + '</i>'
    return wrapper


@strong
@italic
def greeting(name, message):
    return '{} wants to say {}'.format(name, message)


def main():
    print(greeting('Jane', 'I\'m happy'))


if __name__ == '__main__':
    main()
