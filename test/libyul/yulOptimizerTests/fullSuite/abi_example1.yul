{
    let x := abi_encode_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr_to_t_array$_t_array$_t_address_$3_memory_$dyn_memory_ptr(mload(0), 0x20)
    let a, b, c, d := abi_decode_tuple_t_uint256t_uint256t_array$_t_uint256_$dyn_memory_ptrt_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(mload(0x20), mload(0x40))
    sstore(a, b)
    sstore(c, d)
    sstore(0, x)

    function abi_decode_t_address(offset, end) -> value
    {
        value := cleanup_revert_t_address(calldataload(offset))
    }
    function abi_decode_t_array$_t_address_$dyn_memory(offset, end) -> array
    {
        if iszero(slt(add(offset, 0x1f), end))
        {
            revert(0, 0)
        }
        let length := calldataload(offset)
        array := allocateMemory(array_allocation_size_t_array$_t_address_$dyn_memory(length))
        let dst := array
        mstore(array, length)
        offset := add(offset, 0x20)
        dst := add(dst, 0x20)
        let src := offset
        if gt(add(src, mul(length, 0x20)), end)
        {
            revert(0, 0)
        }
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            let elementPos := src
            mstore(dst, abi_decode_t_address(elementPos, end))
            dst := add(dst, 0x20)
            src := add(src, 0x20)
        }
    }
    function abi_decode_t_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(offset, end) -> array
    {
        if iszero(slt(add(offset, 0x1f), end))
        {
            revert(0, 0)
        }
        let length := calldataload(offset)
        array := allocateMemory(array_allocation_size_t_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(length))
        let dst := array
        mstore(array, length)
        offset := add(offset, 0x20)
        dst := add(dst, 0x20)
        let src := offset
        if gt(add(src, mul(length, 0x40)), end)
        {
            revert(0, 0)
        }
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            let elementPos := src
            mstore(dst, abi_decode_t_array$_t_uint256_$2_memory(elementPos, end))
            dst := add(dst, 0x20)
            src := add(src, 0x40)
        }
    }
    function abi_decode_t_array$_t_uint256_$2_memory(offset, end) -> array
    {
        if iszero(slt(add(offset, 0x1f), end))
        {
            revert(0, 0)
        }
        let length := 0x2
        array := allocateMemory(array_allocation_size_t_array$_t_uint256_$2_memory(length))
        let dst := array
        let src := offset
        if gt(add(src, mul(length, 0x20)), end)
        {
            revert(0, 0)
        }
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            let elementPos := src
            mstore(dst, abi_decode_t_uint256(elementPos, end))
            dst := add(dst, 0x20)
            src := add(src, 0x20)
        }
    }
    function abi_decode_t_array$_t_uint256_$dyn_memory(offset, end) -> array
    {
        if iszero(slt(add(offset, 0x1f), end))
        {
            revert(0, 0)
        }
        let length := calldataload(offset)
        array := allocateMemory(array_allocation_size_t_array$_t_uint256_$dyn_memory(length))
        let dst := array
        mstore(array, length)
        offset := add(offset, 0x20)
        dst := add(dst, 0x20)
        let src := offset
        if gt(add(src, mul(length, 0x20)), end)
        {
            revert(0, 0)
        }
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            let elementPos := src
            mstore(dst, abi_decode_t_uint256(elementPos, end))
            dst := add(dst, 0x20)
            src := add(src, 0x20)
        }
    }
    function abi_decode_t_array$_t_uint256_$dyn_memory_ptr(offset, end) -> array
    {
        if iszero(slt(add(offset, 0x1f), end))
        {
            revert(0, 0)
        }
        let length := calldataload(offset)
        array := allocateMemory(array_allocation_size_t_array$_t_uint256_$dyn_memory_ptr(length))
        let dst := array
        mstore(array, length)
        offset := add(offset, 0x20)
        dst := add(dst, 0x20)
        let src := offset
        if gt(add(src, mul(length, 0x20)), end)
        {
            revert(0, 0)
        }
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            let elementPos := src
            mstore(dst, abi_decode_t_uint256(elementPos, end))
            dst := add(dst, 0x20)
            src := add(src, 0x20)
        }
    }
    function abi_decode_t_contract$_C_$55(offset, end) -> value
    {
        value := cleanup_revert_t_contract$_C_$55(calldataload(offset))
    }
    function abi_decode_t_struct$_S_$11_memory_ptr(headStart, end) -> value
    {
        if slt(sub(end, headStart), 0x60)
        {
            revert(0, 0)
        }
        value := allocateMemory(0x60)
        {
            let offset := 0
            mstore(add(value, 0x0), abi_decode_t_uint256(add(headStart, offset), end))
        }
        {
            let offset := calldataload(add(headStart, 32))
            if gt(offset, 0xffffffffffffffff)
            {
                revert(0, 0)
            }
            mstore(add(value, 0x20), abi_decode_t_array$_t_uint256_$dyn_memory(add(headStart, offset), end))
        }
        {
            let offset := calldataload(add(headStart, 64))
            if gt(offset, 0xffffffffffffffff)
            {
                revert(0, 0)
            }
            mstore(add(value, 0x40), abi_decode_t_array$_t_address_$dyn_memory(add(headStart, offset), end))
        }
    }
    function abi_decode_t_uint256(offset, end) -> value
    {
        value := cleanup_revert_t_uint256(calldataload(offset))
    }
    function abi_decode_t_uint8(offset, end) -> value
    {
        value := cleanup_revert_t_uint8(calldataload(offset))
    }
    function abi_decode_tuple_t_contract$_C_$55t_uint8(headStart, dataEnd) -> value0, value1
    {
        if slt(sub(dataEnd, headStart), 64)
        {
            revert(0, 0)
        }
        {
            let offset := 0
            value0 := abi_decode_t_contract$_C_$55(add(headStart, offset), dataEnd)
        }
        {
            let offset := 32
            value1 := abi_decode_t_uint8(add(headStart, offset), dataEnd)
        }
    }
    function abi_decode_tuple_t_struct$_S_$11_memory_ptrt_uint256(headStart, dataEnd) -> value0, value1
    {
        if slt(sub(dataEnd, headStart), 64)
        {
            revert(0, 0)
        }
        {
            let offset := calldataload(add(headStart, 0))
            if gt(offset, 0xffffffffffffffff)
            {
                revert(0, 0)
            }
            value0 := abi_decode_t_struct$_S_$11_memory_ptr(add(headStart, offset), dataEnd)
        }
        {
            let offset := 32
            value1 := abi_decode_t_uint256(add(headStart, offset), dataEnd)
        }
    }
    function abi_decode_tuple_t_uint256t_uint256t_array$_t_uint256_$dyn_memory_ptrt_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(headStart, dataEnd) -> value0, value1, value2, value3
    {
        if slt(sub(dataEnd, headStart), 128)
        {
            revert(0, 0)
        }
        {
            let offset := 0
            value0 := abi_decode_t_uint256(add(headStart, offset), dataEnd)
        }
        {
            let offset := 32
            value1 := abi_decode_t_uint256(add(headStart, offset), dataEnd)
        }
        {
            let offset := calldataload(add(headStart, 64))
            if gt(offset, 0xffffffffffffffff)
            {
                revert(0, 0)
            }
            value2 := abi_decode_t_array$_t_uint256_$dyn_memory_ptr(add(headStart, offset), dataEnd)
        }
        {
            let offset := calldataload(add(headStart, 96))
            if gt(offset, 0xffffffffffffffff)
            {
                revert(0, 0)
            }
            value3 := abi_decode_t_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(add(headStart, offset), dataEnd)
        }
    }
    function abi_encode_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr_to_t_array$_t_array$_t_address_$3_memory_$dyn_memory_ptr(value, pos) -> end
    {
        let length := array_length_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(value)
        mstore(pos, length)
        pos := add(pos, 0x20)
        let srcPtr := array_dataslot_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(value)
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            abi_encode_t_array$_t_contract$_C_$55_$3_memory_to_t_array$_t_address_$3_memory_ptr(mload(srcPtr), pos)
            srcPtr := array_nextElement_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(srcPtr)
            pos := add(pos, 0x60)
        }
        end := pos
    }
    function abi_encode_t_array$_t_contract$_C_$55_$3_memory_to_t_array$_t_address_$3_memory_ptr(value, pos)
    {
        let length := array_length_t_array$_t_contract$_C_$55_$3_memory(value)
        let srcPtr := array_dataslot_t_array$_t_contract$_C_$55_$3_memory(value)
        for {
            let i := 0
        }
        lt(i, length)
        {
            i := add(i, 1)
        }
        {
            abi_encode_t_contract$_C_$55_to_t_address(mload(srcPtr), pos)
            srcPtr := array_nextElement_t_array$_t_contract$_C_$55_$3_memory(srcPtr)
            pos := add(pos, 0x20)
        }
    }
    function abi_encode_t_bool_to_t_bool(value, pos)
    {
        mstore(pos, cleanup_assert_t_bool(value))
    }
    function abi_encode_t_contract$_C_$55_to_t_address(value, pos)
    {
        mstore(pos, convert_t_contract$_C_$55_to_t_address(value))
    }
    function abi_encode_t_uint16_to_t_uint16(value, pos)
    {
        mstore(pos, cleanup_assert_t_uint16(value))
    }
    function abi_encode_t_uint24_to_t_uint24(value, pos)
    {
        mstore(pos, cleanup_assert_t_uint24(value))
    }
    function abi_encode_tuple_t_bool__to_t_bool_(headStart, value0) -> tail
    {
        tail := add(headStart, 32)
        abi_encode_t_bool_to_t_bool(value0, add(headStart, 0))
    }
    function abi_encode_tuple_t_uint16_t_uint24_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr__to_t_uint16_t_uint24_t_array$_t_array$_t_address_$3_memory_$dyn_memory_ptr_(headStart, value2, value1, value0) -> tail
    {
        tail := add(headStart, 96)
        abi_encode_t_uint16_to_t_uint16(value0, add(headStart, 0))
        abi_encode_t_uint24_to_t_uint24(value1, add(headStart, 32))
        mstore(add(headStart, 64), sub(tail, headStart))
        tail := abi_encode_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr_to_t_array$_t_array$_t_address_$3_memory_$dyn_memory_ptr(value2, tail)
    }
    function allocateMemory(size) -> memPtr
    {
        memPtr := mload(64)
        let newFreePtr := add(memPtr, size)
        if or(gt(newFreePtr, 0xffffffffffffffff), lt(newFreePtr, memPtr))
        {
            revert(0, 0)
        }
        mstore(64, newFreePtr)
    }
    function array_allocation_size_t_array$_t_address_$dyn_memory(length) -> size
    {
        if gt(length, 0xffffffffffffffff)
        {
            revert(0, 0)
        }
        size := mul(length, 0x20)
        size := add(size, 0x20)
    }
    function array_allocation_size_t_array$_t_array$_t_uint256_$2_memory_$dyn_memory_ptr(length) -> size
    {
        if gt(length, 0xffffffffffffffff)
        {
            revert(0, 0)
        }
        size := mul(length, 0x20)
        size := add(size, 0x20)
    }
    function array_allocation_size_t_array$_t_uint256_$2_memory(length) -> size
    {
        if gt(length, 0xffffffffffffffff)
        {
            revert(0, 0)
        }
        size := mul(length, 0x20)
    }
    function array_allocation_size_t_array$_t_uint256_$dyn_memory(length) -> size
    {
        if gt(length, 0xffffffffffffffff)
        {
            revert(0, 0)
        }
        size := mul(length, 0x20)
        size := add(size, 0x20)
    }
    function array_allocation_size_t_array$_t_uint256_$dyn_memory_ptr(length) -> size
    {
        if gt(length, 0xffffffffffffffff)
        {
            revert(0, 0)
        }
        size := mul(length, 0x20)
        size := add(size, 0x20)
    }
    function array_dataslot_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(memPtr) -> dataPtr
    {
        dataPtr := add(memPtr, 0x20)
    }
    function array_dataslot_t_array$_t_contract$_C_$55_$3_memory(memPtr) -> dataPtr
    {
        dataPtr := memPtr
    }
    function array_length_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(value) -> length
    {
        length := mload(value)
    }
    function array_length_t_array$_t_contract$_C_$55_$3_memory(value) -> length
    {
        length := 0x3
    }
    function array_nextElement_t_array$_t_array$_t_contract$_C_$55_$3_memory_$dyn_memory_ptr(memPtr) -> nextPtr
    {
        nextPtr := add(memPtr, 0x20)
    }
    function array_nextElement_t_array$_t_contract$_C_$55_$3_memory(memPtr) -> nextPtr
    {
        nextPtr := add(memPtr, 0x20)
    }
    function cleanup_assert_t_address(value) -> cleaned
    {
        cleaned := cleanup_assert_t_uint160(value)
    }
    function cleanup_assert_t_bool(value) -> cleaned
    {
        cleaned := iszero(iszero(value))
    }
    function cleanup_assert_t_uint16(value) -> cleaned
    {
        cleaned := and(value, 0xFFFF)
    }
    function cleanup_assert_t_uint160(value) -> cleaned
    {
        cleaned := and(value, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)
    }
    function cleanup_assert_t_uint24(value) -> cleaned
    {
        cleaned := and(value, 0xFFFFFF)
    }
    function cleanup_revert_t_address(value) -> cleaned
    {
        cleaned := cleanup_assert_t_uint160(value)
    }
    function cleanup_revert_t_contract$_C_$55(value) -> cleaned
    {
        cleaned := cleanup_assert_t_address(value)
    }
    function cleanup_revert_t_uint256(value) -> cleaned
    {
        cleaned := value
    }
    function cleanup_revert_t_uint8(value) -> cleaned
    {
        cleaned := and(value, 0xFF)
    }
    function convert_t_contract$_C_$55_to_t_address(value) -> converted
    {
        converted := convert_t_contract$_C_$55_to_t_uint160(value)
    }
    function convert_t_contract$_C_$55_to_t_uint160(value) -> converted
    {
        converted := cleanup_assert_t_uint160(value)
    }
}
// ----
// fullSuite
// {
//     {
//         let _1 := 0x20
//         let _2 := 0
//         let _244 := mload(_2)
//         let abi_encode_pos := _1
//         let abi_encode_length := mload(_244)
//         mstore(_1, abi_encode_length)
//         let abi_encode_pos_1 := 64
//         abi_encode_pos := abi_encode_pos_1
//         let abi_encode_srcPtr_1 := add(_244, _1)
//         let abi_encode_i_1 := _2
//         for {
//         }
//         lt(abi_encode_i_1, abi_encode_length)
//         {
//             abi_encode_i_1 := add(abi_encode_i_1, 1)
//         }
//         {
//             let _323 := mload(abi_encode_srcPtr_1)
//             let abi_encode_pos_28 := abi_encode_pos
//             let abi_encode_length_2 := 0x3
//             let abi_encode_srcPtr_23 := _323
//             let abi_encode_i_23 := _2
//             for {
//             }
//             lt(abi_encode_i_23, abi_encode_length_2)
//             {
//                 abi_encode_i_23 := add(abi_encode_i_23, 1)
//             }
//             {
//                 mstore(abi_encode_pos_28, and(mload(abi_encode_srcPtr_23), 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF))
//                 abi_encode_srcPtr_23 := add(abi_encode_srcPtr_23, _1)
//                 abi_encode_pos_28 := add(abi_encode_pos_28, _1)
//             }
//             abi_encode_srcPtr_1 := add(abi_encode_srcPtr_1, _1)
//             abi_encode_pos := add(abi_encode_pos, 0x60)
//         }
//         let _325 := 0x40
//         let _246 := mload(_325)
//         let _247 := mload(_1)
//         let abi_decode_value0_2
//         let abi_decode_value0 := abi_decode_value0_2
//         let abi_decode_value1_2
//         let abi_decode_value1 := abi_decode_value1_2
//         let abi_decode_value2_2
//         let abi_decode_value2 := abi_decode_value2_2
//         let abi_decode_value3_2
//         let abi_decode_value3 := abi_decode_value3_2
//         if slt(sub(_246, _247), 128)
//         {
//             revert(_2, _2)
//         }
//         {
//             abi_decode_value0 := calldataload(_247)
//         }
//         {
//             abi_decode_value1 := calldataload(add(_247, 32))
//         }
//         {
//             let abi_decode_offset := calldataload(add(_247, abi_encode_pos_1))
//             let _332 := 0xffffffffffffffff
//             if gt(abi_decode_offset, _332)
//             {
//                 revert(_2, _2)
//             }
//             let _334 := add(_247, abi_decode_offset)
//             if iszero(slt(add(_334, 0x1f), _246))
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_length_3 := calldataload(_334)
//             if gt(abi_decode_length_3, _332)
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_array_allo__7 := mul(abi_decode_length_3, _1)
//             let abi_decode_array_10 := allocateMemory(add(abi_decode_array_allo__7, _1))
//             let abi_decode_dst_36 := abi_decode_array_10
//             mstore(abi_decode_array_10, abi_decode_length_3)
//             let abi_decode_offset_8 := add(_334, _1)
//             abi_decode_dst_36 := add(abi_decode_array_10, _1)
//             let abi_decode_src_30 := abi_decode_offset_8
//             if gt(add(add(_334, abi_decode_array_allo__7), _1), _246)
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_i_30 := _2
//             for {
//             }
//             lt(abi_decode_i_30, abi_decode_length_3)
//             {
//                 abi_decode_i_30 := add(abi_decode_i_30, 1)
//             }
//             {
//                 mstore(abi_decode_dst_36, calldataload(abi_decode_src_30))
//                 abi_decode_dst_36 := add(abi_decode_dst_36, _1)
//                 abi_decode_src_30 := add(abi_decode_src_30, _1)
//             }
//             abi_decode_value2 := abi_decode_array_10
//         }
//         {
//             let abi_decode_offset_1 := calldataload(add(_247, 96))
//             let _337 := 0xffffffffffffffff
//             if gt(abi_decode_offset_1, _337)
//             {
//                 revert(_2, _2)
//             }
//             let _339 := add(_247, abi_decode_offset_1)
//             let abi_decode__76 := 0x1f
//             if iszero(slt(add(_339, abi_decode__76), _246))
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_length_4 := calldataload(_339)
//             if gt(abi_decode_length_4, _337)
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_array_13 := allocateMemory(add(mul(abi_decode_length_4, _1), _1))
//             let abi_decode_dst_40 := abi_decode_array_13
//             mstore(abi_decode_array_13, abi_decode_length_4)
//             let abi_decode_offset_10 := add(_339, _1)
//             abi_decode_dst_40 := add(abi_decode_array_13, _1)
//             let abi_decode_src_33 := abi_decode_offset_10
//             if gt(add(add(_339, mul(abi_decode_length_4, _325)), _1), _246)
//             {
//                 revert(_2, _2)
//             }
//             let abi_decode_i_34 := _2
//             for {
//             }
//             lt(abi_decode_i_34, abi_decode_length_4)
//             {
//                 abi_decode_i_34 := add(abi_decode_i_34, 1)
//             }
//             {
//                 if iszero(slt(add(abi_decode_src_33, abi_decode__76), _246))
//                 {
//                     revert(_2, _2)
//                 }
//                 let abi_decode_abi_decode_length := 0x2
//                 let allocateMe_memPtr_1 := mload(abi_encode_pos_1)
//                 let allocateMe_newFreePtr := add(allocateMe_memPtr_1, abi_encode_pos_1)
//                 if or(gt(allocateMe_newFreePtr, _337), lt(allocateMe_newFreePtr, allocateMe_memPtr_1))
//                 {
//                     revert(_2, _2)
//                 }
//                 mstore(abi_encode_pos_1, allocateMe_newFreePtr)
//                 let abi_decode_abi_decode_dst_1 := allocateMe_memPtr_1
//                 let abi_decode_abi_decode_src_1 := abi_decode_src_33
//                 if gt(add(abi_decode_src_33, abi_encode_pos_1), _246)
//                 {
//                     revert(_2, _2)
//                 }
//                 let abi_decode_abi_decode_i_1 := _2
//                 for {
//                 }
//                 lt(abi_decode_abi_decode_i_1, abi_decode_abi_decode_length)
//                 {
//                     abi_decode_abi_decode_i_1 := add(abi_decode_abi_decode_i_1, 1)
//                 }
//                 {
//                     mstore(abi_decode_abi_decode_dst_1, calldataload(abi_decode_abi_decode_src_1))
//                     abi_decode_abi_decode_dst_1 := add(abi_decode_abi_decode_dst_1, _1)
//                     abi_decode_abi_decode_src_1 := add(abi_decode_abi_decode_src_1, _1)
//                 }
//                 mstore(abi_decode_dst_40, allocateMe_memPtr_1)
//                 abi_decode_dst_40 := add(abi_decode_dst_40, _1)
//                 abi_decode_src_33 := add(abi_decode_src_33, _325)
//             }
//             abi_decode_value3 := abi_decode_array_13
//         }
//         sstore(abi_decode_value0, abi_decode_value1)
//         sstore(abi_decode_value2, abi_decode_value3)
//         sstore(_2, abi_encode_pos)
//     }
//     function allocateMemory(size) -> memPtr
//     {
//         let _199 := 64
//         let memPtr_5 := mload(_199)
//         memPtr := memPtr_5
//         let newFreePtr := add(memPtr_5, size)
//         if or(gt(newFreePtr, 0xffffffffffffffff), lt(newFreePtr, memPtr_5))
//         {
//             let _204 := 0
//             revert(_204, _204)
//         }
//         mstore(_199, newFreePtr)
//     }
// }
