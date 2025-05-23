=============
CI/CD Tooling
=============

Commit Message Linting
======================

The pipeline now contains a commit linting step which uses
`commitlint <https://github.com/conventional-changelog/commitlint>`_ using its
configuration for
`Conventional Commits <https://www.conventionalcommits.org/en/v1.0.0/>`_.

Commitlint is now part of the build docker container and we have linting
wrapper script which will lint all commits that would be merged from your
current branch to a given target branch.

For example:

.. code-block:: sh

    ci/docker-run.sh
    ci/lint-commits.sh origin/integration

will lint all commits from HEAD that would be merged to origin/integration with
a PR. Note that this may not work if your feature branch is not up to date with
the target branch.

Running the Linter Outside Docker
---------------------------------

In order to use the script above outside Docker, you have to install
commitlint.

To install globally on a recent Ubuntu/Debian, you could do (note that this
will install an updated node.js in /usr/local):

.. code-block:: sh

   # install repository npm
   sudo apt install npm
   # install node updater globally
   sudo npm install n -g
   # update node
   sudo n stable
   # install commitlint and conventional commit configuration
   sudo npm install @commitlint/cli -g
   sudo npm install @commitlint/config-conventional -g

This should take care of everything. If you're getting path errors nonetheless,
you may need to set the ``NODE_PATH`` environment variable to
``/usr/local/lib/node_modules``.

Alternatively, if a global installation is not preferred, you can look for
local installation instructions using ``npm``/``npx`` in the commitlint
documentation. You can set the ``COMMITLINT_CMD`` environment variable to
indicate how your local ``commitlint`` should be executed if it is not in path
or if you want to use a wrapper like ``npx``.

Using the Optional Git Hook
---------------------------

There is a ``commit-msg`` hook for linting your commit message in the
``.githooks/`` directory. If you want to use it, you must have installed the
prerequisites to run the ``ci/lint-commits.sh`` script outside of docker, as
described above.

To then activate the git hook, run the following command *from the repo root*:

.. code-block:: sh

   git config --local  core.hooksPath .githooks/

If you want to bypass the hook for e.g. a work-in-progress commit, just use
``git commit -n`` or ``git commit --no-verify``.

Note that the hook will not complain about 'fixup' commits, assuming that you
plan to squash them later. However, the pipeline step will still prevent
'fixup' commits from being merged.

Code-Formatting
===============

The code formatter script ``ci/format-code.sh`` will use ``clang-format`` and
``shfmt`` with the correct settings to format C code, header files and shell
scripts within the project.

Optionally, the ``--check`` parameter does a dry run, reporting errors if any
files do not conform to formatting rules.

The ``clang-format`` and ``shfmt`` tools should be available in most distros.
Alternatively, they are available n the build container as well.
