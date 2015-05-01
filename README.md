#  The exec plugin

A function that runs arbitrary shell programs as the user
that runs the SciDB database.

 *WARNING*

This is a DANGEROUS plugin. Any user who can run queries in SciDB can use this
plugin to run arbitrary programs with it on every node. Programs run with the
permission of the user running SciDB. You can delete files, render the database
unusable, destroy worlds, and generally cause chaos with this plugin. Only use
it experimentally.


## Synopsis

Scalar valued function `exec`:
```
int32 exec (string)
```

Input value:

* A string representing a shell-executable command line statment.

Return value:

* The int32 shell exit code.

## Details

The exec function is a thin wrapper for the C++ standard library `system`
function. That function passes the single argument to the system `sh`
program, which can vary considerably on Unix systems.

The function waits for the shell exit and returns the exit code (of the shell).
Other  programs may be launched into the background with the usual shell
ampersand convention.

## Examples

Load the plugin.
```
iquery -aq "load_library('exec')"
```

Run `pwd` on every instance and record output to the file `/tmp/foo`. Note
that we use `redimension` to change the output chunk size to 1 in order to
run on every instance.

```
iquery -aq "
apply(
  redimension(
    apply( list('instances'), command, 'pwd >> /tmp/foo'),
    <command:string>[No=0:*,1,0]),
  ret, exec(command))
"

# Running this on every node produces output like:
cat /tmp/foo 
/home/scidb/000/0
/home/scidb/000/1
```

The next example uses a simple hashing function in the
https://github.com/paradigm4/superfunpack plugin to run a shell command only
once on each node in the cluster.

```
iquery -aq "load_library('superfunpack')"

iquery -aq "
apply(
  apply(
    redimension(
      redimension(
        apply(
          project(
            apply(list('instances'), node, substr(name,strlen(name)-7,strlen(name))),
            node),
          h,dumb_hash(node)),
        <node:string null, No:int64>[h=0:*,1,0], min(node) as node),
      <node:string null>[No=0:*,1,0]),
    command, 'hostname >> /tmp/foo'),
  ret, exec(command))"
```

## Installation

If you have the https://github.com/paradigm4/dev_tools plugin installed, you can simply run:
```
iquery -aq "load_library('dev_tools')"
iquery -aq "install_github('paradigm4/exec')"
```

Otherwise, clone this repository, run `Make` and copy the `libexec.so` file to the
`/opt/scidb/<version>/lib/scidb/plugins` directory on every node. 
