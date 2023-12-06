for line in open("./building-types.txt").readlines():
    line = line.strip()
    print(f'<type id="{line}" db-per-cut="9" db-per-meter="0.4" />')
