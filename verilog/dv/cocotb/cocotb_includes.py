from os import path
import sys

sys.path.append(path.abspath('/home/rady/user_projects/aes/caravel-sim-infrastructure/cocotb'))

from interfaces.common_functions.test_functions import report_test
from interfaces.common_functions.test_functions import test_configure
from interfaces.UART import UART
from interfaces.SPI import SPI

import cocotb
