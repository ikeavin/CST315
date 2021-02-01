Created by Kevin Ahlstrom for CST 315, taught by William Hurst
at Grand Canyon University.

My approach to the consumer producer problem is to have a buffer
which consumer consumes from and the producer produces into.
Whenever a thread is producing or consuming, a mutex lock is
placed preventing the other threads from interacting with the
buffer. Semaphores are also used to determine if the buffer is
full or empty, and to avoid consuming when empty and avoid
producing when full.
