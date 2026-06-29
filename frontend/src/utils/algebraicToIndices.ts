const algebraicToIndices = (square: string): [number, number] => {
    const lut: { [key: string]: number } = {"a": 0, "b": 1, "c": 2, "d": 3, "e": 4, "f": 5, "g": 6, "h": 7}
    const col = square[0]
    const row = square[1]
    return [8 - Number(row), lut[col]]
}

export default algebraicToIndices