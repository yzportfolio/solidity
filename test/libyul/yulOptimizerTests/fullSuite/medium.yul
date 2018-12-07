{
    function allocate(size) -> p {
        p := mload(0x40)
        mstore(0x40, add(p, size))
    }
    function array_index_access(array, index) -> p {
        p := add(array, mul(index, 0x20))
    }
    pop(allocate(0x20))
    let x := allocate(0x40)
    mstore(array_index_access(x, 3), 2)
    if 0 {
        mstore(0x40, 0x20)
    }
    if sub(2,1) {
        for { switch mul(1,2) case 2 { mstore(0x40, 0x20) } } sub(1,1) {} { mstore(0x80, 0x40) }
    }
}
// ----
// fullSuite
// {
//     {
//         let _1 := 0x20
//         let allocate_ := 0x40
//         mstore(allocate_, add(mload(allocate_), _1))
//         let allocate_p_4 := mload(allocate_)
//         mstore(allocate_, add(allocate_p_4, allocate_))
//         mstore(add(allocate_p_4, 96), 2)
//         mstore(allocate_, _1)
//     }
// }
