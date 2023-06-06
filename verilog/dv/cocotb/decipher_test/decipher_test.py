import cocotb
from cocotb.triggers import FallingEdge, RisingEdge, ClockCycles
import cocotb.log
from cocotb_includes import test_configure
from cocotb_includes import report_test


@cocotb.test()
@report_test
async def decipher_test(dut):

    caravelEnv = await test_configure(dut, timeout_cycles=465722)
    await caravelEnv.wait_mgmt_gpio(1)
    code = caravelEnv.monitor_gpio(2,0).integer
    if code != 0x7:
        cocotb.log.error(f"[TEST] first reading is wrong with in phase {code}")
    else: 
        cocotb.log.info("[TEST] first reading is correct")
    await caravelEnv.wait_mgmt_gpio(0)
    code = caravelEnv.monitor_gpio(2,0).integer
    if code != 0x7:
        cocotb.log.error(f"[TEST] second reading is wrong with in phase {code}")
    else: 
        cocotb.log.info("[TEST] second reading is correct")
