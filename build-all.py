from subprocess import call

# Compile the kernel
call(["make", "compile"], cwd="kernel")
