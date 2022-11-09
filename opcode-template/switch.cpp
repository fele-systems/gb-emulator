#include <gbcpu.h>
#include <opcodes-autogen.h>

InstructionTrait GbCpu::execute_next()
{
    auto instruction = static_cast<Opcode> ( next_pc_byte() );
    switch ( instruction ) {

    
        ////////////////////////////////////////////////////////////////////////
        // 8-Bit Loads
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Load 8-Bit Immediate to 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_8imm:
                    return LD_r8_8imm (R8::A);
                case Opcode::LD_B_8imm:
                    return LD_r8_8imm (R8::B);
                case Opcode::LD_C_8imm:
                    return LD_r8_8imm (R8::C);
                case Opcode::LD_D_8imm:
                    return LD_r8_8imm (R8::D);
                case Opcode::LD_E_8imm:
                    return LD_r8_8imm (R8::E);
                case Opcode::LD_H_8imm:
                    return LD_r8_8imm (R8::H);
                case Opcode::LD_L_8imm:
                    return LD_r8_8imm (R8::L);
        
            //----------------------------------------------------------------------
            // Load 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_A:
                    return LD_r8_r8 (R8::A, R8::A);
                case Opcode::LD_A_B:
                    return LD_r8_r8 (R8::A, R8::B);
                case Opcode::LD_A_C:
                    return LD_r8_r8 (R8::A, R8::C);
                case Opcode::LD_A_D:
                    return LD_r8_r8 (R8::A, R8::D);
                case Opcode::LD_A_E:
                    return LD_r8_r8 (R8::A, R8::E);
                case Opcode::LD_A_H:
                    return LD_r8_r8 (R8::A, R8::H);
                case Opcode::LD_A_L:
                    return LD_r8_r8 (R8::A, R8::L);
                case Opcode::LD_B_A:
                    return LD_r8_r8 (R8::B, R8::A);
                case Opcode::LD_B_B:
                    return LD_r8_r8 (R8::B, R8::B);
                case Opcode::LD_B_C:
                    return LD_r8_r8 (R8::B, R8::C);
                case Opcode::LD_B_D:
                    return LD_r8_r8 (R8::B, R8::D);
                case Opcode::LD_B_E:
                    return LD_r8_r8 (R8::B, R8::E);
                case Opcode::LD_B_H:
                    return LD_r8_r8 (R8::B, R8::H);
                case Opcode::LD_B_L:
                    return LD_r8_r8 (R8::B, R8::L);
                case Opcode::LD_C_A:
                    return LD_r8_r8 (R8::C, R8::A);
                case Opcode::LD_C_B:
                    return LD_r8_r8 (R8::C, R8::B);
                case Opcode::LD_C_C:
                    return LD_r8_r8 (R8::C, R8::C);
                case Opcode::LD_C_D:
                    return LD_r8_r8 (R8::C, R8::D);
                case Opcode::LD_C_E:
                    return LD_r8_r8 (R8::C, R8::E);
                case Opcode::LD_C_H:
                    return LD_r8_r8 (R8::C, R8::H);
                case Opcode::LD_C_L:
                    return LD_r8_r8 (R8::C, R8::L);
                case Opcode::LD_D_A:
                    return LD_r8_r8 (R8::D, R8::A);
                case Opcode::LD_D_B:
                    return LD_r8_r8 (R8::D, R8::B);
                case Opcode::LD_D_C:
                    return LD_r8_r8 (R8::D, R8::C);
                case Opcode::LD_D_D:
                    return LD_r8_r8 (R8::D, R8::D);
                case Opcode::LD_D_E:
                    return LD_r8_r8 (R8::D, R8::E);
                case Opcode::LD_D_H:
                    return LD_r8_r8 (R8::D, R8::H);
                case Opcode::LD_D_L:
                    return LD_r8_r8 (R8::D, R8::L);
                case Opcode::LD_E_A:
                    return LD_r8_r8 (R8::E, R8::A);
                case Opcode::LD_E_B:
                    return LD_r8_r8 (R8::E, R8::B);
                case Opcode::LD_E_C:
                    return LD_r8_r8 (R8::E, R8::C);
                case Opcode::LD_E_D:
                    return LD_r8_r8 (R8::E, R8::D);
                case Opcode::LD_E_E:
                    return LD_r8_r8 (R8::E, R8::E);
                case Opcode::LD_E_H:
                    return LD_r8_r8 (R8::E, R8::H);
                case Opcode::LD_E_L:
                    return LD_r8_r8 (R8::E, R8::L);
                case Opcode::LD_H_A:
                    return LD_r8_r8 (R8::H, R8::A);
                case Opcode::LD_H_B:
                    return LD_r8_r8 (R8::H, R8::B);
                case Opcode::LD_H_C:
                    return LD_r8_r8 (R8::H, R8::C);
                case Opcode::LD_H_D:
                    return LD_r8_r8 (R8::H, R8::D);
                case Opcode::LD_H_E:
                    return LD_r8_r8 (R8::H, R8::E);
                case Opcode::LD_H_H:
                    return LD_r8_r8 (R8::H, R8::H);
                case Opcode::LD_H_L:
                    return LD_r8_r8 (R8::H, R8::L);
                case Opcode::LD_L_A:
                    return LD_r8_r8 (R8::L, R8::A);
                case Opcode::LD_L_B:
                    return LD_r8_r8 (R8::L, R8::B);
                case Opcode::LD_L_C:
                    return LD_r8_r8 (R8::L, R8::C);
                case Opcode::LD_L_D:
                    return LD_r8_r8 (R8::L, R8::D);
                case Opcode::LD_L_E:
                    return LD_r8_r8 (R8::L, R8::E);
                case Opcode::LD_L_H:
                    return LD_r8_r8 (R8::L, R8::H);
                case Opcode::LD_L_L:
                    return LD_r8_r8 (R8::L, R8::L);
        
            //----------------------------------------------------------------------
            // Load byte at HL into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_pHL:
                    return LD_r8_pr16 (R8::A, R16::HL);
                case Opcode::LD_B_pHL:
                    return LD_r8_pr16 (R8::B, R16::HL);
                case Opcode::LD_C_pHL:
                    return LD_r8_pr16 (R8::C, R16::HL);
                case Opcode::LD_D_pHL:
                    return LD_r8_pr16 (R8::D, R16::HL);
                case Opcode::LD_E_pHL:
                    return LD_r8_pr16 (R8::E, R16::HL);
                case Opcode::LD_H_pHL:
                    return LD_r8_pr16 (R8::H, R16::HL);
                case Opcode::LD_L_pHL:
                    return LD_r8_pr16 (R8::L, R16::HL);
        
            //----------------------------------------------------------------------
            // Load 8-Bit value into byte at HL
            //----------------------------------------------------------------------
            
                case Opcode::LD_pHL_B:
                    return LD_pr16_r8 (R16::HL, R8::B);
                case Opcode::LD_pHL_C:
                    return LD_pr16_r8 (R16::HL, R8::C);
                case Opcode::LD_pHL_D:
                    return LD_pr16_r8 (R16::HL, R8::D);
                case Opcode::LD_pHL_E:
                    return LD_pr16_r8 (R16::HL, R8::E);
                case Opcode::LD_pHL_H:
                    return LD_pr16_r8 (R16::HL, R8::H);
                case Opcode::LD_pHL_L:
                    return LD_pr16_r8 (R16::HL, R8::L);
                case Opcode::LD_pHL_8imm:
                    return LD_pr16_8imm (R16::HL);
        
            //----------------------------------------------------------------------
            // Loads byte at 16-Bit Register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_pBC:
                    return LD_r8_pr16 (R8::A, R16::BC);
                case Opcode::LD_A_pDE:
                    return LD_r8_pr16 (R8::A, R16::DE);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit register into byte at 16-Bit Register
            //----------------------------------------------------------------------
            
                case Opcode::LD_pBC_A:
                    return LD_pr16_r8 (R16::BC, R8::A);
                case Opcode::LD_pDE_A:
                    return LD_pr16_r8 (R16::DE, R8::A);
                case Opcode::LD_pHL_A:
                    return LD_pr16_r8 (R16::HL, R8::A);
        
            //----------------------------------------------------------------------
            // Loads byte at 16-Bit Immediate into 8-Bit Register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_pp16imm:
                    return LD_r8_p16imm (R8::A);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit Register into byte at 16-Bit Register
            //----------------------------------------------------------------------
            
                case Opcode::LD_pp16imm_A:
                    return LD_p16imm_r8 (R8::A);
        
            //----------------------------------------------------------------------
            // Loads byte at (0xFF00 + 8-Bit Register) into 8-Bit Register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_pC:
                    return LD_r8_pr8 (R8::A, R8::C);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit register into byte at (0xFF00 + 8-Bit Register)
            //----------------------------------------------------------------------
            
                case Opcode::LD_pC_A:
                    return LD_pr8_r8 (R8::C, R8::A);
        
            //----------------------------------------------------------------------
            // Loads byte at 16-Bit Register into 8-Bit register. Decrement 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LDD_A_pHL:
                    return LDD_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit register into byte at 16-Bit Register Decrement 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LDD_pHL_A:
                    return LDD_pr16_r8 (R16::HL, R8::A);
        
            //----------------------------------------------------------------------
            // Loads byte at 16-Bit Register into 8-Bit register. Increment 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LDI_A_pHL:
                    return LDI_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit register into byte at 16-Bit Register Increment 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LDI_pHL_A:
                    return LDI_pr16_r8 (R16::HL, R8::A);
        
            //----------------------------------------------------------------------
            // Loads byte at (0xFF00 + 8-Bit immediate) into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_A_pp8imm:
                    return LD_r8_p8imm (R8::A);
        
            //----------------------------------------------------------------------
            // Loads 8-Bit register into byte at (0xFF00 + 8-Bit Immediate)
            //----------------------------------------------------------------------
            
                case Opcode::LD_pp8imm_A:
                    return LD_p8imm_r8 (R8::A);
        
    
        ////////////////////////////////////////////////////////////////////////
        // 16-Bit Loads
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Loads 16-Bit immediate into 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_BC_16imm:
                    return LD_r16_16imm (R16::BC);
                case Opcode::LD_DE_16imm:
                    return LD_r16_16imm (R16::DE);
                case Opcode::LD_HL_16imm:
                    return LD_r16_16imm (R16::HL);
                case Opcode::LD_SP_16imm:
                    return LD_r16_16imm (R16::SP);
        
            //----------------------------------------------------------------------
            // Loads 16-Bit register into 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_SP_HL:
                    return LD_r16_r16 (R16::SP, R16::HL);
        
            //----------------------------------------------------------------------
            // Loads 16-Bit register + 8-Bit immediate into 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::LD_HL_SP_8imm:
                    return LD_r16_r16_8imm (R16::HL, R16::SP);
        
            //----------------------------------------------------------------------
            // Loads 16-Bit register into memory at 16-Bit immediate
            //----------------------------------------------------------------------
            
                case Opcode::LD_16imm_SP:
                    return LD_16imm_r16 (R16::SP);
        
            //----------------------------------------------------------------------
            // Push 16-Bit register onto stack. Decrememt SP twice
            //----------------------------------------------------------------------
            
                case Opcode::PUSH_AF:
                    return PUSH_r16 (R16::AF);
                case Opcode::PUSH_BC:
                    return PUSH_r16 (R16::BC);
                case Opcode::PUSH_DE:
                    return PUSH_r16 (R16::DE);
                case Opcode::PUSH_HL:
                    return PUSH_r16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Pop 16-Bit off stack into 16-Bit register. Incrememt SP twice
            //----------------------------------------------------------------------
            
                case Opcode::POP_AF:
                    return POP_r16 (R16::AF);
                case Opcode::POP_BC:
                    return POP_r16 (R16::BC);
                case Opcode::POP_DE:
                    return POP_r16 (R16::DE);
                case Opcode::POP_HL:
                    return POP_r16 (R16::HL);
        
    
        ////////////////////////////////////////////////////////////////////////
        // 8-Bit ALU
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Add 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADD_A_A:
                    return ADD_r8_r8 (R8::A, R8::A);
                case Opcode::ADD_A_B:
                    return ADD_r8_r8 (R8::A, R8::B);
                case Opcode::ADD_A_C:
                    return ADD_r8_r8 (R8::A, R8::C);
                case Opcode::ADD_A_D:
                    return ADD_r8_r8 (R8::A, R8::D);
                case Opcode::ADD_A_E:
                    return ADD_r8_r8 (R8::A, R8::E);
                case Opcode::ADD_A_H:
                    return ADD_r8_r8 (R8::A, R8::H);
                case Opcode::ADD_A_L:
                    return ADD_r8_r8 (R8::A, R8::L);
        
            //----------------------------------------------------------------------
            // Add 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADD_A_pHL:
                    return ADD_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Add 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADD_A_8imm:
                    return ADD_r8_8imm (R8::A);
        
            //----------------------------------------------------------------------
            // Add 8-Bit register + carry flag into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADC_A_A:
                    return ADC_r8_r8 (R8::A, R8::A);
                case Opcode::ADC_A_B:
                    return ADC_r8_r8 (R8::A, R8::B);
                case Opcode::ADC_A_C:
                    return ADC_r8_r8 (R8::A, R8::C);
                case Opcode::ADC_A_D:
                    return ADC_r8_r8 (R8::A, R8::D);
                case Opcode::ADC_A_E:
                    return ADC_r8_r8 (R8::A, R8::E);
                case Opcode::ADC_A_H:
                    return ADC_r8_r8 (R8::A, R8::H);
                case Opcode::ADC_A_L:
                    return ADC_r8_r8 (R8::A, R8::L);
        
            //----------------------------------------------------------------------
            // Add 8-Bit + carry flag at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADC_A_pHL:
                    return ADC_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Add 8-Bit immediate + carry flag into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADC_A_8imm:
                    return ADC_r8_8imm (R8::A);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SUB_A_A:
                    return SUB_r8_r8 (R8::A, R8::A);
                case Opcode::SUB_A_B:
                    return SUB_r8_r8 (R8::A, R8::B);
                case Opcode::SUB_A_C:
                    return SUB_r8_r8 (R8::A, R8::C);
                case Opcode::SUB_A_D:
                    return SUB_r8_r8 (R8::A, R8::D);
                case Opcode::SUB_A_E:
                    return SUB_r8_r8 (R8::A, R8::E);
                case Opcode::SUB_A_H:
                    return SUB_r8_r8 (R8::A, R8::H);
                case Opcode::SUB_A_L:
                    return SUB_r8_r8 (R8::A, R8::L);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SUB_A_pHL:
                    return SUB_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SUB_A_8imm:
                    return SUB_r8_8imm (R8::A);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit register + carry flag into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SBC_A_A:
                    return SBC_r8_r8 (R8::A, R8::A);
                case Opcode::SBC_A_B:
                    return SBC_r8_r8 (R8::A, R8::B);
                case Opcode::SBC_A_C:
                    return SBC_r8_r8 (R8::A, R8::C);
                case Opcode::SBC_A_D:
                    return SBC_r8_r8 (R8::A, R8::D);
                case Opcode::SBC_A_E:
                    return SBC_r8_r8 (R8::A, R8::E);
                case Opcode::SBC_A_H:
                    return SBC_r8_r8 (R8::A, R8::H);
                case Opcode::SBC_A_L:
                    return SBC_r8_r8 (R8::A, R8::L);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit + carry flag at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SBC_A_pHL:
                    return SBC_r8_pr16 (R8::A, R16::HL);
        
            //----------------------------------------------------------------------
            // Subtracts 8-Bit immediate + carry flag into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SBC_A_8imm:
                    return SBC_r8_8imm (R8::A);
        
            //----------------------------------------------------------------------
            // Logically ANDs 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::AND_A:
                    return AND_r8 (R8::A);
                case Opcode::AND_B:
                    return AND_r8 (R8::B);
                case Opcode::AND_C:
                    return AND_r8 (R8::C);
                case Opcode::AND_D:
                    return AND_r8 (R8::D);
                case Opcode::AND_E:
                    return AND_r8 (R8::E);
                case Opcode::AND_H:
                    return AND_r8 (R8::H);
                case Opcode::AND_L:
                    return AND_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Logically ANDs 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::AND_HL:
                    return AND_r8 (R8::HL);
        
            //----------------------------------------------------------------------
            // Logically ANDs 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::AND_8imm:
                    return AND_8imm ();
        
            //----------------------------------------------------------------------
            // Logically ORs 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::OR_A:
                    return OR_r8 (R8::A);
                case Opcode::OR_B:
                    return OR_r8 (R8::B);
                case Opcode::OR_C:
                    return OR_r8 (R8::C);
                case Opcode::OR_D:
                    return OR_r8 (R8::D);
                case Opcode::OR_E:
                    return OR_r8 (R8::E);
                case Opcode::OR_H:
                    return OR_r8 (R8::H);
                case Opcode::OR_L:
                    return OR_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Logically ORs 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::OR_HL:
                    return OR_r8 (R8::HL);
        
            //----------------------------------------------------------------------
            // Logically ORs 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::OR_8imm:
                    return OR_8imm ();
        
            //----------------------------------------------------------------------
            // Logically XORs 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::XOR_A:
                    return XOR_r8 (R8::A);
                case Opcode::XOR_B:
                    return XOR_r8 (R8::B);
                case Opcode::XOR_C:
                    return XOR_r8 (R8::C);
                case Opcode::XOR_D:
                    return XOR_r8 (R8::D);
                case Opcode::XOR_E:
                    return XOR_r8 (R8::E);
                case Opcode::XOR_H:
                    return XOR_r8 (R8::H);
                case Opcode::XOR_L:
                    return XOR_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Logically XORs 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::XOR_HL:
                    return XOR_r8 (R8::HL);
        
            //----------------------------------------------------------------------
            // Logically XORs 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::XOR_8imm:
                    return XOR_8imm ();
        
            //----------------------------------------------------------------------
            // Compares 8-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::CP_A:
                    return CP_r8 (R8::A);
                case Opcode::CP_B:
                    return CP_r8 (R8::B);
                case Opcode::CP_C:
                    return CP_r8 (R8::C);
                case Opcode::CP_D:
                    return CP_r8 (R8::D);
                case Opcode::CP_E:
                    return CP_r8 (R8::E);
                case Opcode::CP_H:
                    return CP_r8 (R8::H);
                case Opcode::CP_L:
                    return CP_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Compares 8-Bit at 16-Bit register into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::CP_HL:
                    return CP_r8 (R8::HL);
        
            //----------------------------------------------------------------------
            // Compares 8-Bit immediate into 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::CP_8imm:
                    return CP_8imm ();
        
            //----------------------------------------------------------------------
            // Increments 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::INC_A:
                    return INC_r8 (R8::A);
                case Opcode::INC_B:
                    return INC_r8 (R8::B);
                case Opcode::INC_C:
                    return INC_r8 (R8::C);
                case Opcode::INC_D:
                    return INC_r8 (R8::D);
                case Opcode::INC_E:
                    return INC_r8 (R8::E);
                case Opcode::INC_H:
                    return INC_r8 (R8::H);
                case Opcode::INC_L:
                    return INC_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Increments value at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::INC_pHL:
                    return INC_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Decrements 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::DEC_A:
                    return DEC_r8 (R8::A);
                case Opcode::DEC_B:
                    return DEC_r8 (R8::B);
                case Opcode::DEC_C:
                    return DEC_r8 (R8::C);
                case Opcode::DEC_D:
                    return DEC_r8 (R8::D);
                case Opcode::DEC_E:
                    return DEC_r8 (R8::E);
                case Opcode::DEC_H:
                    return DEC_r8 (R8::H);
                case Opcode::DEC_L:
                    return DEC_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Decrements value at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::DEC_pHL:
                    return DEC_pr16 (R16::HL);
        
    
        ////////////////////////////////////////////////////////////////////////
        // 16-Bit ALU
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Add two 16-Bit Registers
            //----------------------------------------------------------------------
            
                case Opcode::ADD_HL_BC:
                    return ADD_r16_r16 (R16::HL, R16::BC);
                case Opcode::ADD_HL_DE:
                    return ADD_r16_r16 (R16::HL, R16::DE);
                case Opcode::ADD_HL_HL:
                    return ADD_r16_r16 (R16::HL, R16::HL);
                case Opcode::ADD_HL_SP:
                    return ADD_r16_r16 (R16::HL, R16::SP);
        
            //----------------------------------------------------------------------
            // Add 8-Bit immediate to 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::ADD_SP_8imm:
                    return ADD_r16_8imm (R16::SP);
        
            //----------------------------------------------------------------------
            // Increments 16-Bit Registers
            //----------------------------------------------------------------------
            
                case Opcode::INC_BC:
                    return INC_r16 (R16::BC);
                case Opcode::INC_DE:
                    return INC_r16 (R16::DE);
                case Opcode::INC_HL:
                    return INC_r16 (R16::HL);
                case Opcode::INC_SP:
                    return INC_r16 (R16::SP);
        
            //----------------------------------------------------------------------
            // Decrements 16-Bit Registers
            //----------------------------------------------------------------------
            
                case Opcode::DEC_BC:
                    return DEC_r16 (R16::BC);
                case Opcode::DEC_DE:
                    return DEC_r16 (R16::DE);
                case Opcode::DEC_HL:
                    return DEC_r16 (R16::HL);
                case Opcode::DEC_SP:
                    return DEC_r16 (R16::SP);
        
    
        ////////////////////////////////////////////////////////////////////////
        // Miscellaneous
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Swap upper and lower nibles of 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SWAP_A:
                    return SWAP_r8 (R8::A);
                case Opcode::SWAP_B:
                    return SWAP_r8 (R8::B);
                case Opcode::SWAP_C:
                    return SWAP_r8 (R8::C);
                case Opcode::SWAP_D:
                    return SWAP_r8 (R8::D);
                case Opcode::SWAP_E:
                    return SWAP_r8 (R8::E);
                case Opcode::SWAP_H:
                    return SWAP_r8 (R8::H);
                case Opcode::SWAP_L:
                    return SWAP_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Swap upper and lower nibles of memory at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SWAP_pHL:
                    return SWAP_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Decimal adjust register A so that the correct BCD is obtained
            //----------------------------------------------------------------------
            
                case Opcode::DAA:
                    return DAA ();
        
            //----------------------------------------------------------------------
            // Complement A register (flip all bits)
            //----------------------------------------------------------------------
            
                case Opcode::CPL:
                    return CPL ();
        
            //----------------------------------------------------------------------
            // Complement carry flag (C = !C)
            //----------------------------------------------------------------------
            
                case Opcode::CCF:
                    return CCF ();
        
            //----------------------------------------------------------------------
            // Set carry flag
            //----------------------------------------------------------------------
            
                case Opcode::SCF:
                    return SCF ();
        
            //----------------------------------------------------------------------
            // No operation
            //----------------------------------------------------------------------
            
                case Opcode::NOP:
                    return NOP ();
        
            //----------------------------------------------------------------------
            // Halt CPU until interrupt
            //----------------------------------------------------------------------
            
                case Opcode::HALT:
                    return HALT ();
        
            //----------------------------------------------------------------------
            // Halt CPU and LCD Display until button pressed
            //----------------------------------------------------------------------
            
                case Opcode::STOP:
                    return STOP ();
        
            //----------------------------------------------------------------------
            // Disable interrupt
            //----------------------------------------------------------------------
            
                case Opcode::DI:
                    return DI ();
        
            //----------------------------------------------------------------------
            // Enable interrupt
            //----------------------------------------------------------------------
            
                case Opcode::EI:
                    return EI ();
        
    
        ////////////////////////////////////////////////////////////////////////
        // Rotates and Shifts
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Rotate A left. Old bit 7 to Carry flag
            //----------------------------------------------------------------------
            
                case Opcode::RLCA:
                    return RLCA ();
        
            //----------------------------------------------------------------------
            // Rotate A left through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RLA:
                    return RLA ();
        
            //----------------------------------------------------------------------
            // Rotate A right. Old bit 7 to Carry flag
            //----------------------------------------------------------------------
            
                case Opcode::RRCA:
                    return RRCA ();
        
            //----------------------------------------------------------------------
            // Rotate A right through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RRA:
                    return RRA ();
        
            //----------------------------------------------------------------------
            // Rotate 8-Bit register left. Old bit 7 to Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RLC_A:
                    return RLC_r8 (R8::A);
                case Opcode::RLC_B:
                    return RLC_r8 (R8::B);
                case Opcode::RLC_C:
                    return RLC_r8 (R8::C);
                case Opcode::RLC_D:
                    return RLC_r8 (R8::D);
                case Opcode::RLC_E:
                    return RLC_r8 (R8::E);
                case Opcode::RLC_H:
                    return RLC_r8 (R8::H);
                case Opcode::RLC_L:
                    return RLC_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Rotate memory at 16-Bit register left. Old bit 7 to Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RLC_pHL:
                    return RLC_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Rotate 8-Bit register left through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RL_A:
                    return RL_r8 (R8::A);
                case Opcode::RL_B:
                    return RL_r8 (R8::B);
                case Opcode::RL_C:
                    return RL_r8 (R8::C);
                case Opcode::RL_D:
                    return RL_r8 (R8::D);
                case Opcode::RL_E:
                    return RL_r8 (R8::E);
                case Opcode::RL_H:
                    return RL_r8 (R8::H);
                case Opcode::RL_L:
                    return RL_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Rotate memory at 16-Bit register left through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RL_pHL:
                    return RL_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Rotate 8-Bit register right. Old bit 7 to Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RRC_A:
                    return RRC_r8 (R8::A);
                case Opcode::RRC_B:
                    return RRC_r8 (R8::B);
                case Opcode::RRC_C:
                    return RRC_r8 (R8::C);
                case Opcode::RRC_D:
                    return RRC_r8 (R8::D);
                case Opcode::RRC_E:
                    return RRC_r8 (R8::E);
                case Opcode::RRC_H:
                    return RRC_r8 (R8::H);
                case Opcode::RRC_L:
                    return RRC_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Rotate memory at 16-Bit register right. Old bit 7 to Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RRC_pHL:
                    return RRC_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Rotate 8-Bit register right through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RR_A:
                    return RR_r8 (R8::A);
                case Opcode::RR_B:
                    return RR_r8 (R8::B);
                case Opcode::RR_C:
                    return RR_r8 (R8::C);
                case Opcode::RR_D:
                    return RR_r8 (R8::D);
                case Opcode::RR_E:
                    return RR_r8 (R8::E);
                case Opcode::RR_H:
                    return RR_r8 (R8::H);
                case Opcode::RR_L:
                    return RR_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Rotate memory at 16-Bit register right through Carry Flag
            //----------------------------------------------------------------------
            
                case Opcode::RR_pHL:
                    return RR_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Shift 8-Bit register left into Carry Flag. LSB set to 0
            //----------------------------------------------------------------------
            
                case Opcode::SLA_A:
                    return SLA_r8 (R8::A);
                case Opcode::SLA_B:
                    return SLA_r8 (R8::B);
                case Opcode::SLA_C:
                    return SLA_r8 (R8::C);
                case Opcode::SLA_D:
                    return SLA_r8 (R8::D);
                case Opcode::SLA_E:
                    return SLA_r8 (R8::E);
                case Opcode::SLA_H:
                    return SLA_r8 (R8::H);
                case Opcode::SLA_L:
                    return SLA_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Shift memory at 16-Bit register left into Carry Flag. LSB set to 0
            //----------------------------------------------------------------------
            
                case Opcode::SLA_pHL:
                    return SLA_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Shift 8-Bit register right into Carry Flag. MSB doesn't change
            //----------------------------------------------------------------------
            
                case Opcode::SRA_A:
                    return SRA_r8 (R8::A);
                case Opcode::SRA_B:
                    return SRA_r8 (R8::B);
                case Opcode::SRA_C:
                    return SRA_r8 (R8::C);
                case Opcode::SRA_D:
                    return SRA_r8 (R8::D);
                case Opcode::SRA_E:
                    return SRA_r8 (R8::E);
                case Opcode::SRA_H:
                    return SRA_r8 (R8::H);
                case Opcode::SRA_L:
                    return SRA_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Shift memory at 16-Bit register right into Carry Flag. MSB doesn't change
            //----------------------------------------------------------------------
            
                case Opcode::SRA_pHL:
                    return SRA_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Shift 8-Bit register right into Carry Flag. MSB set to 0
            //----------------------------------------------------------------------
            
                case Opcode::SRL_A:
                    return SRL_r8 (R8::A);
                case Opcode::SRL_B:
                    return SRL_r8 (R8::B);
                case Opcode::SRL_C:
                    return SRL_r8 (R8::C);
                case Opcode::SRL_D:
                    return SRL_r8 (R8::D);
                case Opcode::SRL_E:
                    return SRL_r8 (R8::E);
                case Opcode::SRL_H:
                    return SRL_r8 (R8::H);
                case Opcode::SRL_L:
                    return SRL_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Shift memory at 16-Bit register right into Carry Flag. MSB set to 0
            //----------------------------------------------------------------------
            
                case Opcode::SRL_pHL:
                    return SRL_pr16 (R16::HL);
        
    
        ////////////////////////////////////////////////////////////////////////
        // Bit Opcodes
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Test bit b in 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::BIT_8imm_A:
                    return BIT_8imm_r8 (R8::A);
                case Opcode::BIT_8imm_B:
                    return BIT_8imm_r8 (R8::B);
                case Opcode::BIT_8imm_C:
                    return BIT_8imm_r8 (R8::C);
                case Opcode::BIT_8imm_D:
                    return BIT_8imm_r8 (R8::D);
                case Opcode::BIT_8imm_E:
                    return BIT_8imm_r8 (R8::E);
                case Opcode::BIT_8imm_H:
                    return BIT_8imm_r8 (R8::H);
                case Opcode::BIT_8imm_L:
                    return BIT_8imm_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Test bit b in memory at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SRL_pHL:
                    return SRL_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Set bit b in 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SET_8imm_A:
                    return SET_8imm_r8 (R8::A);
                case Opcode::SET_8imm_B:
                    return SET_8imm_r8 (R8::B);
                case Opcode::SET_8imm_C:
                    return SET_8imm_r8 (R8::C);
                case Opcode::SET_8imm_D:
                    return SET_8imm_r8 (R8::D);
                case Opcode::SET_8imm_E:
                    return SET_8imm_r8 (R8::E);
                case Opcode::SET_8imm_H:
                    return SET_8imm_r8 (R8::H);
                case Opcode::SET_8imm_L:
                    return SET_8imm_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // set bit b in memory at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::SET_pHL:
                    return SET_pr16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Reset bit b in 8-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::RES_8imm_A:
                    return RES_8imm_r8 (R8::A);
                case Opcode::RES_8imm_B:
                    return RES_8imm_r8 (R8::B);
                case Opcode::RES_8imm_C:
                    return RES_8imm_r8 (R8::C);
                case Opcode::RES_8imm_D:
                    return RES_8imm_r8 (R8::D);
                case Opcode::RES_8imm_E:
                    return RES_8imm_r8 (R8::E);
                case Opcode::RES_8imm_H:
                    return RES_8imm_r8 (R8::H);
                case Opcode::RES_8imm_L:
                    return RES_8imm_r8 (R8::L);
        
            //----------------------------------------------------------------------
            // Reset bit b in memory at 16-Bit register
            //----------------------------------------------------------------------
            
                case Opcode::RES_pHL:
                    return RES_pr16 (R16::HL);
        
    
        ////////////////////////////////////////////////////////////////////////
        // Jumps
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Jump to address in 16-Bit immediate
            //----------------------------------------------------------------------
            
                case Opcode::JP_16imm:
                    return JP_16imm ();
                case Opcode::JP_NZ_16imm:
                    return JP_NZ_16imm ();
                case Opcode::JP_Z_16imm:
                    return JP_Z_16imm ();
                case Opcode::JP_NC_16imm:
                    return JP_NC_16imm ();
                case Opcode::JP_C_16imm:
                    return JP_C_16imm ();
                case Opcode::JP_HL:
                    return JP_r16 (R16::HL);
        
            //----------------------------------------------------------------------
            // Jump to address relative to current address
            //----------------------------------------------------------------------
            
                case Opcode::JP_8imm:
                    return JP_8imm ();
                case Opcode::JP_NZ_8imm:
                    return JP_NZ_8imm ();
                case Opcode::JP_Z_8imm:
                    return JP_Z_8imm ();
                case Opcode::JP_NC_8imm:
                    return JP_NC_8imm ();
                case Opcode::JP_C_8imm:
                    return JP_C_8imm ();
        
    
        ////////////////////////////////////////////////////////////////////////
        // Calls
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Push address of nexus instruction onto stack and then jump to address
            //----------------------------------------------------------------------
            
                case Opcode::CALL_16imm:
                    return CALL_16imm ();
                case Opcode::CALL_NZ_16imm:
                    return CALL_NZ_16imm ();
                case Opcode::CALL_Z_16imm:
                    return CALL_Z_16imm ();
                case Opcode::CALL_NC_16imm:
                    return CALL_NC_16imm ();
                case Opcode::CALL_C_16imm:
                    return CALL_C_16imm ();
        
    
        ////////////////////////////////////////////////////////////////////////
        // Returns
        ////////////////////////////////////////////////////////////////////////
        
            //----------------------------------------------------------------------
            // Pop two bytes from stack and jump to that address
            //----------------------------------------------------------------------
            
                case Opcode::RET:
                    return RET ();
                case Opcode::RETI:
                    return RETI ();
                case Opcode::RET_NZ:
                    return RET_NZ ();
                case Opcode::RET_Z:
                    return RET_Z ();
                case Opcode::RET_NC:
                    return RET_NC ();
                case Opcode::RET_C:
                    return RET_C ();
        
    
    }
}