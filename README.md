# Computer science stuff

## Content

- [Computer science stuff](#computer-science-stuff)
  - [Content](#content)
  - [Algorithms](#algorithms)
  - [Containers](#containers)
  - [Design patterns](#design-patterns)
    - [Behavioral patterns](#behavioral-patterns)
    - [Creational patterns](#creational-patterns)
    - [Structural patterns](#structural-patterns)

## Algorithms

- [Huffman compression](./algorithms/huffman)

## Containers

- [Whatever(any)](./containers/whatever) – type-safe any type container.

- [Thread pool](./containers/thread_pool)

- [Variant](./containers/variant)

- [Big natural number](./containers/bignum)

## Design patterns

### Behavioral patterns

| Pattern                                                                                                                                                           | Description                                                                                                                                                      |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Chain of responsibility ([c++](./design_patterns/behavioral/chain-of-responsibility/c++/chain_of_responsibility.cpp))                                             | used to process varied requests, each of which may be dealt with by a different handler                                                                          |
| Command ([c++](./design_patterns/behavioral/command/c++/command.cpp))                                                                                             | used to express a request, including the call to be made and all of its required parameters, in a command object                                                 |
| Interpreter ([c++](./design_patterns/behavioral/interpreter/c++/interpreter.cpp))                                                                                 | used to evaluate sentences in a language                                                                                                                         |
| Iterator ([c++](./design_patterns/behavioral/iterator/c++/iterator.cpp), [swift](./design_patterns/behavioral/iterator/swift/iterator.playground/Contents.swift)) | used to provide a standard interface for traversing a collection of items in an aggregate object without the need to understand its underlying structure         |
| Mediator ([c++](./design_patterns/behavioral/mediator/c++/mediator.cpp))                                                                                          | used to reduce coupling between classes that communicate with each other                                                                                         |
| Memento ([c++](./design_patterns/behavioral/memento/c++/memento.cpp))                                                                                             | used to capture the current state of an object and store it in such a manner that it can be restored at a later time without breaking the rules of encapsulation |
| Observer ([c++](./design_patterns/behavioral/observer/c++/observer.cpp))                                                                                          | used to allow an object to publish changes to its state. Other objects subscribe to be immediately notified of any changes                                       |
| State ([c++](./design_patterns/behavioral/state/c++/state.cpp))                                                                                                   | used to alter the behaviour of an object as its internal state changes. The pattern allows the class for an object to apparently change at run-time              |
| Strategy ([c++](./design_patterns/behavioral/strategy/c++/strategy.cpp))                                                                                          | used to create an interchangeable family of algorithms from which the required process is chosen at run-time                                                     |
| Template method ([c++](./design_patterns/behavioral/template-method/c++/template.cpp))                                                                            | used when two or more implementations of an algorithm exist                                                                                                      |
| Visitor ([c++](./design_patterns/behavioral/visitor/c++/visitor.cpp))                                                                                             | used to separate a relatively complex set of structured data classes from the functionality that may be performed upon the data that they hold                   |

### Creational patterns

| Pattern                                                                                                                                                            | Description                                                                                                                                                        |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Abstract factory ([c++](./design_patterns/creational/abstract-factory/c++/abstract_factory.cpp))                                                                   | used to provide a client with a set of related or dependant objects. The "family" of objects created by the factory are determined at run-time                     |
| Builder ([c++](./design_patterns/creational/builder/c++/builder.cpp))                                                                                              | used to create complex objects with constituent parts that must be created in the same order or using a specific algorithm                                         |
| Factory ([c++](./design_patterns/creational/factory/c++/factory.cpp))                                                                                              | used to replace class constructors, abstracting the process of object generation so that the type of the object instantiated can be determined at run-time         |
| Prototype ([c++](./design_patterns/creational/prototype/c++/prototype.cpp))                                                                                        | used to instantiate a new object by copying all of the properties of an existing object, creating an independent clone                                             |
| Singleton ([c++](./design_patterns/creational/singleton/c++/old/singleton.cpp), [c++11](./design_patterns/creational/singleton/c++/new_thread_safe/singleton.cpp)) | ensures that only one object of a particular class is ever created. All further references to objects of the singleton class refer to the same underlying instance |

### Structural patterns

| Pattern                                                                                                                                           | Description                                                                                                                                                                    |
| ------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Adapter ([c++](./design_patterns/structural/adapter/c++/adapter.cpp))                                                                             | used to provide a link between two otherwise incompatible types by wrapping the "adaptee" with a class that supports the interface required by the client                      |
| Bridge ([c++](./design_patterns/structural/bridge/c++/bridge.cpp))                                                                                | used to separate the abstract elements of a class from the implementation details, providing the means to replace the implementation details without modifying the abstraction |
| Composite ([c++](./design_patterns/structural/composite/c++/composite.cpp))                                                                       | used to create hierarchical, recursive tree structures of related objects where any element of the structure may be accessed and utilised in a standard manner                 |
| Decorator ([c++](./design_patterns/structural/decorator/c++/decorator.cpp), [python](./design_patterns/structural/decorator/python/decorator.py)) | used to extend or alter the functionality of objects at run- time by wrapping them in an object of a decorator class                                                           |
| Facade ([c++](./design_patterns/structural/facade/c++/facade.cpp))                                                                                | used to define a simplified interface to a more complex subsystem                                                                                                              |
| Flyweight ([c++](./design_patterns/structural/flyweight/c++/flyweight.cpp))                                                                       | used to minimize memory usage or computational expenses by sharing as much as possible with other similar objects                                                              |
| Proxy ([c++](./design_patterns/structural/proxy/c++/proxy.cpp))                                                                                   | used to provide a surrogate or placeholder object, which references an underlying object                                                                                       |
