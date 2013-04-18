import os
from os.path import join
import commands


def skip_file(name):
  skip = [".git", "build", "cmake", "dashboard", "dependencies", "doxygen", "ice", "tests", "ui"]
  for s in skip:
    if name.startswith("./" + s):
      return True
  return False

source_files = []
for root, dirs, files in os.walk('.'):
  if not skip_file(root):
    for f in files:
      source_files.append(join(root, f))

out = commands.getoutput("python cmake/cpplint.py %s" % (" ".join(source_files)))
for line in out.split("\n"):
  if not line.startswith("Done") and not line.startswith("Ignoring"):
    print line
