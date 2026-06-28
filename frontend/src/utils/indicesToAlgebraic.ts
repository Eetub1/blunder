const indicesToAlgebraic = (indices: [number, number]): string => {
    const [row, col] = indices
    const lut: { [key: number]: string } = {0: "a", 1: "b", 2: "c", 3: "d", 4: "e", 5: "f", 6: "g", 7: "h"}
    return lut[col] + (8 - row).toString()
}

export default indicesToAlgebraic