const maxApi = require('max-api');
const fs = require('fs');
maxApi.addHandler('newFolder',(path) => {
    // Verificar si la carpeta no existe y crearla
    if (!fs.existsSync(path)) {
        fs.mkdirSync(path, { recursive: true }, (err) => {
            if (err) {
                maxApi.post('Error al crear la carpeta:');
            } else {
                maxApi.post('Carpeta creada exitosamente.');
                maxApi.outletBang();
            }
        });
    } else {
        maxApi.post('La carpeta ya existe.');
        maxApi.outletBang();
    }
});

