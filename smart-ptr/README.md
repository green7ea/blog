# Pointers on C++'s smart pointers

> Sorry for the title, I just couldn't help myself.

## TL;DR

Don't use smart pointers in function parameters unless you're taking
ownership (shared or exclusive).

Return stack objects instead of smart pointers when possible.

```c++
// bad
int count(const std::shared_ptr<std::string> &s, char d);

// good for read only access
int count(const std::string &s, char d);

// good for making a copy
void take_copy(std:shared_ptr<std:string> &&s);
```

Avoid using pointers, the following is the preference for creating
objects:

| Pointer type           | Comment                                                                                                                         |
|------------------------|---------------------------------------------------------------------------------------------------------------------------------|
| `T` (stack allocation) | Best option                                                                                                                     |
| `unique_ptr<T>`        | If you really need a stack allocation                                                                                           |
| `shared_ptr<const T>`  | Not great, but sometimes necessary                                                                                              |
| `T *`                  | When working with C code, shouldn't be exposed long [I.11](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-raw) |
| `shared_ptr<T>`        | Be careful with this one, avoid                                                                                                 |

## Smart pointers as parameters

Let's take the example from the TL;DR, and explain why having smart
pointers in the interface is a bad idea.

```c++
// bad
int count_bad(const std::shared_ptr<std::string> &s, char d);

// good
int count_good(const std::string &s, char d);
```

In this use case, counting is only reading the string, not making a
copy of the shared pointer.

The first thing to mention is that they are both easily callable if I
have a smart pointer object.

```c++
auto shared = std::make_shared<std::string>("Hello shared world!");

int bad = count_bad(shared, 'l');

// Calling the non-smart pointer is as easy as adding a *
int good = count_good(*shared, 'l');
```

They're both as easy to call if I'm already using the same smart
pointer type as the function but much harder to call if I'm not. This
puts pressure on the caller to store their objects in a smart pointer
and in the worst case, all objects end up being stored in smart
pointers even when they don't have to be.

Using smart pointers as a parameter also creates a link between the
function parameter and the outside object: if the function changes
from a `unique_ptr` to a `shared_ptr` the calling code as to be
updated. There's the option to rewrap the object or switch to a
`shared_ptr` — neither of these are good choices.

When you use smart pointers, you're leaving performance on the table —
you rule out faster options like using the stack or an arena
alocator. Most of the time you don't need the performance but it's
nice to be able to get it without major refactoring when you need it.

Lastly, the intention of a function using a smart pointer is also
unclear. It could be making a copy for later use or it could be simply
reading the pointer's values during the call. With a `const
shared_ptr<T> &`, both are possible, with a `const T &`, it's clear
that we're only reading values.

I hope I've convinced you that if you only need to read a parameter of
type `T`, use `const T &` instead of `const shared_ptr<T> &` — only
take `const shared_ptr<T> &` if you are taking a copy of the shared
pointer for future use. There are no downsides but many upsides:

1. easier to call,
2. better decoupling between caller and callee,
3. can be faster when needed,
4. clearer intention.

All of this to avoid having to use `operator*`!

## Const correctness

If you aren't yet convinced, the bad version of the interface doesn't
give you the `const` protections you might expect:

```c++
auto shared = std::make_shared<std::string>("Hello shared world!");

printf("Good count\n");
int good = count_good(*shared, 'l');
printf("'%s' contains %i 'l's.\n\n", shared->c_str(), good);

printf("Bad count\n");
int bad = count_bad(shared, 'l');
printf("'%s' contains %i 'l's.\n\n", shared->c_str(), bad);
```

Prints

```
Good count
'Hello shared world!' contains 3 'l's.

Bad count
'He**o shared wor*d!' contains 3 'l's.
```

Notice the `*`s where the `l`s used to be for the bad count. The
object that is pointed to in a `const shared_ptr<T>` can be modified!
It's the same with a unique pointer.

A `const shared_ptr<T>` doesn't protect the `T` from being modified
but a `shared_ptr<const T>` does. This is done because C++ requires a
copy constructor to work with a `const` object — doing otherwise
breaks basic functionality like returning an object via a temporary.

## The shared_ptr const trick

We can still use `const` to our advantage by using the
`shared_ptr<const T>` type to restrict modification. This is a really
useful trick to overcome the `shared_ptr`'s biggest limitation — loss
of control as to where and when it can change.

We're always told to [Avoid non-const global variables (I.2)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-global) and
the reasoning is:

> Non-const global variables hide dependencies and make the
> dependencies subject to unpredictable changes.

`const shared_ptr<T>` have the same problem as a global variable — any
code that can get a hold of the pointer can make an unpredictable
change.

Let's say we have a race condition on the data held in a `const
shared_ptr<T>`, we have to figure out:

> Which thread modifies the shared pointer?

You can't easily answer that question without auditing the code that
can access the pointer. Imagine having to fix a race condition with a
shared pointer!

`const` global variables don't have the same weakness as the
`non-const` global variables — the equivalent for `shared_ptr` is
`shared_ptr<const T>`.

Shared pointers have an extra trick where you can have a
`shared_ptr<T>` and give everyone else a `shared_ptr<const T>`. It
becomes easier to audit where the data can be modified.

Here's a simple example, we have a configuration that we load from a
file and want to watch for changes. We hand out the configuration to
many parts of our program via a `shared_ptr<const T>` so they get
updates when the configuration changes. In the example, only the
`ConfigWatcher` can update the configuration object and no one else.

```c++
class ConfigWatcher {
public:
  /**
   * By returning a shared_ptr to a const Config, we make sure that
   * we're the only ones that can modify the underlying configuration.
   */
  std::shared_ptr<const Config> get_config() {
    return config;
  }

  /**
   * This could be triggered when the config is updated.
   */
  void update_config() {
    // Code that can modify config can only be in this class.
  }

private:
  std::shared_ptr<Config> config;
};
```

## Conclusion

Smart pointers in C++ are nuanced — they are a powerful tool that have
shortcomings. They make heap allocation much safer but heap allocation
shouldn't become the default or a lot of C++'s usefullness is left on
the table.
