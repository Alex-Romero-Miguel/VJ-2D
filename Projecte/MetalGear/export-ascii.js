// importante copiarlo a C:\Users\ARM\AppData\Local\Tiled\extensions\export-ascii.js para que Tiled lo pueda utilizar
tiled.registerMapFormat("binmap", {
    name: "Byte Tilemap",
    extension: "txt",

    write: (map, fileName) => {
        const layer = map.layerAt(0);
        const tileset = map.tilesets[0];

        // Nombre de archivo relativo
        const fullPath = tileset.image || "unknown.png";
        const fileNameOnly = fullPath.replace(/^.*[\\\/]/, "");
        const relativePath = "images/" + fileNameOnly;

        // Columnas y filas
        const tsCols = Math.floor(tileset.imageWidth / tileset.tileWidth);
        const tsRows = Math.floor(tileset.imageHeight / tileset.tileHeight);

        // Cabecera
        let header = "";
        header += "TILEMAP\n";
        header += `${map.width} ${map.height}\n`;
        header += `${map.tileWidth} ${map.tileHeight}\n`;
        header += relativePath + "\n";
        header += `${tsCols} ${tsRows}\n`;

        // Tiles
        let data = "";
        for (let y = 0; y < map.height; ++y) {
            for (let x = 0; x < map.width; ++x) {
                const tile = layer.tileAt(x, y);
                const id = tile ? tile.id + 1 : 0;
                data += id + " ";
            }
            data += "\n";
        }

        // Escritura
        const file = new BinaryFile(fileName, BinaryFile.WriteOnly);
        file.write(header);
        file.write(data);
        file.commit();
        console.log(`Mapa exportado correctamente a ${fileName}`);
    }
});
