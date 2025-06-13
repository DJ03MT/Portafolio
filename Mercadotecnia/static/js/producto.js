document.addEventListener('DOMContentLoaded', function() {
    // Product Image Gallery
    const thumbnails = document.querySelectorAll('.thumbnail');
    const mainImage = document.getElementById('main-product-image');
    
    thumbnails.forEach(thumb => {
        thumb.addEventListener('click', function() {
            // Remove active class from all thumbnails
            thumbnails.forEach(t => t.classList.remove('active'));
            // Add active class to clicked thumbnail
            this.classList.add('active');
            // Change main image
            mainImage.src = this.src;
            mainImage.alt = this.alt;
        });
    });
    
    // Color Selection
    const colorOptions = document.querySelectorAll('.color-option');
    colorOptions.forEach(option => {
        option.addEventListener('click', function() {
            colorOptions.forEach(opt => opt.classList.remove('active'));
            this.classList.add('active');
            // Here you would typically update the product image based on color
            console.log('Selected color:', this.dataset.color);
        });
    });
    
    // Quantity Selector
    const quantityInput = document.querySelector('input[type="number"]');
    const incrementBtn = document.getElementById('increment');
    const decrementBtn = document.getElementById('decrement');
    
    incrementBtn.addEventListener('click', function() {
        quantityInput.stepUp();
        updateCartButton();
    });
    
    decrementBtn.addEventListener('click', function() {
        if (quantityInput.value > 1) {
            quantityInput.stepDown();
            updateCartButton();
        }
    });
    
    quantityInput.addEventListener('change', function() {
        if (this.value < 1) this.value = 1;
        updateCartButton();
    });
    
    function updateCartButton() {
        console.log('Quantity updated to:', quantityInput.value);
    }
    
    // Smooth scrolling for anchor links
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            e.preventDefault();
            
            const targetId = this.getAttribute('href');
            if (targetId === '#') return;
            
            const targetElement = document.querySelector(targetId);
            if (targetElement) {
                window.scrollTo({
                    top: targetElement.offsetTop - 80,
                    behavior: 'smooth'
                });
            }
        });
    });
    
    // Initialize tooltips
    const tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="tooltip"]'));
    tooltipTriggerList.map(function(tooltipTriggerEl) {
        return new bootstrap.Tooltip(tooltipTriggerEl);
    });
    
    // Form submission
    const contactForm = document.querySelector('.contact-box form');
    if (contactForm) {
        contactForm.addEventListener('submit', function(e) {
            e.preventDefault();
            alert('Gracias por tu mensaje. Nos pondremos en contacto contigo pronto.');
            this.reset();
        });
    }
    
    // Newsletter subscription
    const newsletterForm = document.querySelector('footer form');
    if (newsletterForm) {
        newsletterForm.addEventListener('submit', function(e) {
            e.preventDefault();
            const emailInput = this.querySelector('input[type="email"]');
            alert(`Gracias por suscribirte con ${emailInput.value}. Te enviaremos nuestras novedades pronto.`);
            emailInput.value = '';
        });
    }
});