document.addEventListener('DOMContentLoaded', function() {
    // Mostrar/ocultar campos de tarjeta de crédito
    const paymentMethods = document.querySelectorAll('input[name="paymentMethod"]');
    const creditCardFields = document.getElementById('creditCardFields');
    
    paymentMethods.forEach(method => {
        method.addEventListener('change', function() {
            if (this.id === 'creditCard') {
                creditCardFields.style.display = 'block';
            } else {
                creditCardFields.style.display = 'none';
            }
        });
    });
    
    // Formatear número de tarjeta
    const cardNumberInput = document.getElementById('cardNumber');
    if (cardNumberInput) {
        cardNumberInput.addEventListener('input', function(e) {
            let value = this.value.replace(/\s+/g, '');
            if (value.length > 0) {
                value = value.match(new RegExp('.{1,4}', 'g')).join(' ');
            }
            this.value = value;
        });
    }
    
    // Formatear fecha de expiración
    const cardExpiryInput = document.getElementById('cardExpiry');
    if (cardExpiryInput) {
        cardExpiryInput.addEventListener('input', function(e) {
            let value = this.value.replace(/\D/g, '');
            if (value.length >= 2) {
                value = value.substring(0, 2) + '/' + value.substring(2, 4);
            }
            this.value = value;
        });
    }
    
    // Manejar cambios en el pedido
    const saveChangesBtn = document.getElementById('save-changes');
    if (saveChangesBtn) {
        saveChangesBtn.addEventListener('click', function() {
            // Actualizar resumen con los nuevos valores
            document.getElementById('selected-color').textContent = 'Color: ' + 
                document.getElementById('edit-color').options[document.getElementById('edit-color').selectedIndex].text;
            
            document.getElementById('selected-material').textContent = 'Material: ' + 
                document.getElementById('edit-material').value;
            
            const packageSelect = document.getElementById('edit-package');
            const packageText = packageSelect.options[packageSelect.selectedIndex].text.split(' - ')[0];
            const packagePrice = packageSelect.options[packageSelect.selectedIndex].text.split(' - ')[1];
            
            document.getElementById('selected-package').textContent = packageText;
            document.getElementById('package-price').textContent = packagePrice;
            
            document.getElementById('selected-quantity').textContent = 
                document.getElementById('edit-quantity').value;
            
            document.getElementById('shipping-method').textContent = 
                document.getElementById('edit-shipping').options[document.getElementById('edit-shipping').selectedIndex].text;
            
            // Calcular nuevo total
            const quantity = parseInt(document.getElementById('edit-quantity').value);
            const price = parseFloat(packagePrice.replace('$', '').replace(',', ''));
            const shipping = document.getElementById('edit-shipping').value === 'express' ? 15 : 0;
            const total = (price * quantity) + shipping;
            
            document.getElementById('total-price').textContent = '$' + total.toLocaleString('en-US', {
                minimumFractionDigits: 2,
                maximumFractionDigits: 2
            });
            
            // Cerrar el panel de edición
            bootstrap.Collapse.getInstance(document.getElementById('editOrder')).hide();
            
            // Mostrar notificación
            const alert = document.createElement('div');
            alert.className = 'alert alert-success alert-dismissible fade show mt-3';
            alert.innerHTML = `
                <i class="fas fa-check-circle me-2"></i>
                Pedido actualizado correctamente
                <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
            `;
            document.querySelector('.container').prepend(alert);
        });
    }
    
    // Validación del formulario
    const paymentForm = document.getElementById('paymentForm');
    if (paymentForm) {
        paymentForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Validación básica
            let isValid = true;
            const requiredFields = this.querySelectorAll('[required]');
            
            requiredFields.forEach(field => {
                if (!field.value.trim()) {
                    field.classList.add('is-invalid');
                    isValid = false;
                } else {
                    field.classList.remove('is-invalid');
                }
            });
            
            if (isValid) {
                // Simular procesamiento de pago
                const submitBtn = this.querySelector('button[type="submit"]');
                submitBtn.disabled = true;
                submitBtn.innerHTML = '<i class="fas fa-spinner fa-spin me-2"></i>Procesando pago...';
                
                // Simular retraso de red
                setTimeout(() => {
                    window.location.href = "{{ url_for('confirmacion') }}";
                }, 2000);
            }
        });
    }
    
    // Inicializar tooltips
    const tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="tooltip"]'));
    tooltipTriggerList.map(function(tooltipTriggerEl) {
        return new bootstrap.Tooltip(tooltipTriggerEl);
    });
});