# Installing Python package data by relative path

<time id="last-modified">2018-04-02</time>
<tags>programming, python</tags>

<p id="summary">
Package data installation sometimes requires balance between ease of writing installation procecures for it
and ease of accessing that data. That's especially apparent when someone who is not a developer wants to be
able to edit that data in place. Editing it in place is a bad practice of course, but sometimes that's just
what you get. For example, if there are two people of whom one is a developer who wrote code solely from data format
specification, and the other understands what the data actually means but has no coding skills.
</p>

Python's setuptools, as such, makes installing data files easy. You just need to use the `data_files` argument
of `setup` that is a list of tuples containing destination directory and a list of files to install in it.

So far so good, but that's where the contradictory nature of the goals comes into play. If you specify an
absolute path, that's easy to edit in place, but unfriendly to virtualenvs. If you specify a relative path,
the data ends up in the package directory or egg, where it can be conveniently accessed through the resource
manager, but it's not as easy to find where the data actually is after installation.

So, a quick and dirty solution: access the files by a path relative to `sys.prefix`. And, since setuptools scripts
are themselves written in Python, and also have access to `sys.prefix`, create an absolute path from `sys.prefix`
on the fly:


```python
import sys
import os

setup(name='mypackage',
      version='0.1',
      # ...
      data_files = [(os.path.join(sys.prefix, 'share/mypackage/'),
        ['data/lenna.tiff',
         'data/eicar.txt',
         'data/llama.mp3'])])
```

This way if the end user installs it in production, it will be in a predictable location such as `/usr/share/mypackage`,
but if you install in a virtualenv, it will be something like `/tmp/myenv/share/mypackage`.

Sadly I couldn't find a quick and dirty way to tell setuptools to install all file in a directory rather than list them all
one by one. Perhaps if I get a lot of data, it's better to go for a proper solution and make a separate data package.
