// Efecto de scroll suave adicional (por si el navegador no lo soporta bien)
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener("click", function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute("href"));
        if (target) {
            target.scrollIntoView({
                behavior: "smooth"
            });
        }
    });
});

// Mostrar una alerta al presionar "Reservar ahora"
const reservarBtn = document.querySelector(".btn-outline-success");
if (reservarBtn) {
    reservarBtn.addEventListener("click", () => {
        alert("Gracias por tu interés. ¡Muy pronto disponible para reserva!");
    });
}
