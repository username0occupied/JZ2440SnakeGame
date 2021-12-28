
.text
.global _start

_start:
	b reset          /* vector 0 : reset */
	b halt /* vector 4 : und */
	b halt  /* vector 8 : swi */
	b halt			 /* vector 0x0c : prefetch aboot */
	b halt			 /* vector 0x10 : data abort */
	b halt			 /* vector 0x14 : reserved */
	ldr pc, irq_addr /* vector 0x18 : irq */
	b halt			 /* vector 0x1c : fiq */



irq_addr:
	.word do_irq




.align 4
.align 4

do_irq:
	/* 执行到这里之前:
	 * 1. lr_irq保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_irq保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10010, 进入到irq模式
	 * 4. 跳到0x18的地方执行程序 
	 */

	/* sp_irq未设置, 先设置它 */
	ldr sp, =0x33d00000

	/* 保存现场 */
	/* 在irq异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr-4是异常处理完后的返回地址, 也要保存 */
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	/* 处理irq异常 */
	bl handle_irq_c
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr_irq的值恢复到cpsr里 */


reset:
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
	 *  m = MDIV+8 = 92+8=100
	 *  p = PDIV+2 = 1+2 = 3
	 *  s = SDIV = 1
	 *  FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1)=400M
	 */
	ldr r0, =0x4C000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

	/* 一旦设置PLL, 就会锁定lock time直到PLL输出稳定
	 * 然后CPU工作于新的频率FCLK
	 */
	 //revise

	moveq sp, #4096  /* nand启动 */


	bl sdram_init
	//bl sdram_init2	 /* 用到有初始值的数组, 不是位置无关码 */

	/* 重定位text, rodata, data段整个程序 */
	bl copy2sdram

	/* 清除BSS段 */
	bl clean_bss

	/* 复位之后, cpu处于svc模式
	 * 现在, 切换到usr模式
	 */
	mrs r0, cpsr         /* 读出cpsr */
	bic r0, r0, #0xf     /* 修改M4-M0为0b10000, 进入usr模式 */
	bic r0, r0, #(1<<7)  /* 清除I位, 使能中断 */
	msr cpsr, r0

	/* 设置 sp_usr */
	ldr sp, =0x33f00000

	ldr pc, =sdram
sdram:
	bl uart0_init

und_code:
	//bl main  /* 使用BL命令相对跳转, 程序仍然在NOR/sram执行 */
	ldr lr, =halt
	ldr pc, =main  /* 绝对跳转, 跳到SDRAM */

halt:
	b halt
	
