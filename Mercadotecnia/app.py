from flask import Flask, render_template, request, redirect, url_for, flash, make_response, send_file
from flask_sqlalchemy import SQLAlchemy
from fpdf import FPDF
import datetime
import os
import io 
from fpdf import FPDF

app = Flask(__name__)
app.config['SECRET_KEY'] = 'tu_clave_secreta_aqui'


# Configuración de SQLite
basedir = os.path.abspath(os.path.dirname(__file__))
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + os.path.join(basedir, 'naturasilla.db')
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

# Definición de modelos
class Cliente(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    nombre = db.Column(db.String(100), nullable=False)
    correo = db.Column(db.String(100), nullable=False, unique=True)
    telefono = db.Column(db.String(20), nullable=False)
    direccion = db.Column(db.Text)
    ciudad = db.Column(db.String(50))
    codigo_postal = db.Column(db.String(10))
    pais = db.Column(db.String(50))
    fecha_registro = db.Column(db.DateTime, server_default=db.func.now())

class Producto(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    nombre = db.Column(db.String(100), nullable=False)
    descripcion = db.Column(db.Text)
    precio = db.Column(db.Float, nullable=False)
    categoria = db.Column(db.String(50))
    stock = db.Column(db.Integer, default=0)
    imagen = db.Column(db.String(255))

class Pedido(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    cliente_id = db.Column(db.Integer, db.ForeignKey('cliente.id'), nullable=False)
    fecha_pedido = db.Column(db.DateTime, server_default=db.func.now())
    estado = db.Column(db.String(20), default='pendiente')
    total = db.Column(db.Float, nullable=False)
    metodo_pago = db.Column(db.String(50), nullable=False)
    direccion_envio = db.Column(db.Text, nullable=False)
    cliente = db.relationship('Cliente', backref='pedidos')

class DetallePedido(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    pedido_id = db.Column(db.Integer, db.ForeignKey('pedido.id'), nullable=False)
    producto_id = db.Column(db.Integer, db.ForeignKey('producto.id'), nullable=False)
    cantidad = db.Column(db.Integer, nullable=False)
    precio_unitario = db.Column(db.Float, nullable=False)
    color = db.Column(db.String(50))
    material = db.Column(db.String(50))
    paquete = db.Column(db.String(50))
    pedido = db.relationship('Pedido', backref='detalles')
    producto = db.relationship('Producto')

class Reserva(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    cliente_id = db.Column(db.Integer, db.ForeignKey('cliente.id'))
    nombre = db.Column(db.String(100), nullable=False)
    correo = db.Column(db.String(100), nullable=False)
    telefono = db.Column(db.String(20), nullable=False)
    modelo = db.Column(db.String(50), nullable=False)
    cantidad = db.Column(db.Integer, nullable=False)
    mensaje = db.Column(db.Text)
    fecha_reserva = db.Column(db.DateTime, server_default=db.func.now())
    estado = db.Column(db.String(20), default='pendiente')
    cliente = db.relationship('Cliente', backref='reservas')

# Crear las tablas
with app.app_context():
    db.create_all()
    # Insertar datos iniciales si no existen
    if not Producto.query.first():
        productos_iniciales = [
            Producto(nombre='MovaSmart Pro', descripcion='Silla inteligente con sensores biométricos', precio=1299.00, categoria='premium', stock=50),
            Producto(nombre='MovaSmart Eco', descripcion='Versión ecológica con materiales sostenibles', precio=899.00, categoria='standard', stock=100),
            Producto(nombre='MovaSmart Bambú', descripcion='Diseño en bambú 100% natural', precio=1099.00, categoria='premium', stock=30)
        ]
        db.session.bulk_save_objects(productos_iniciales)
        db.session.commit()


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/beneficios')
def beneficios():
    return render_template('beneficios.html')

@app.route('/producto')
def producto():
    return render_template('producto.html')


# Ruta para procesar reservas
@app.route('/reservar', methods=['GET', 'POST'])
def reservar():
    if request.method == 'POST':
        try:
            reserva = Reserva(
                nombre=request.form['nombre'],
                correo=request.form['correo'],
                telefono=request.form['telefono'],
                modelo=request.form['modelo'],
                cantidad=request.form['cantidad'],
                mensaje=request.form.get('mensaje', '')
            )
            db.session.add(reserva)
            db.session.commit()
            flash('¡Reserva realizada con éxito!', 'success')
            return redirect(url_for('reservar'))
        except Exception as e:
            db.session.rollback()
            flash(f'Error al realizar la reserva: {str(e)}', 'danger')
    
    return render_template('reservar.html')

@app.route('/checkout', methods=['GET', 'POST'])
def checkout():
    if request.method == 'POST':
        try:
            # Crear o encontrar cliente
            cliente = Cliente.query.filter_by(correo=request.form['correo']).first()
            if not cliente:
                cliente = Cliente(
                    nombre=request.form['nombre'],
                    correo=request.form['correo'],
                    telefono=request.form['telefono'],
                    direccion=request.form['direccion'],
                    ciudad=request.form['ciudad'],
                    codigo_postal=request.form['codigo_postal'],
                    pais=request.form['pais']
                )
                db.session.add(cliente)
                db.session.commit()

            # Obtener datos del producto (ejemplo con producto ID 1)
            producto = Producto.query.get(1)
            if not producto:
                flash('Producto no encontrado', 'danger')
                return redirect(url_for('checkout'))

            # Calcular total
            cantidad = int(request.form.get('cantidad', 1))
            total = producto.precio * cantidad

            # Crear pedido
            pedido = Pedido(
                cliente_id=cliente.id,
                total=total,
                metodo_pago=request.form['metodo_pago'],
                direccion_envio=f"{request.form['direccion']}, {request.form['ciudad']}, {request.form['pais']} {request.form['codigo_postal']}"
            )
            db.session.add(pedido)
            db.session.commit()

            # Crear detalle del pedido
            detalle = DetallePedido(
                pedido_id=pedido.id,
                producto_id=producto.id,
                cantidad=cantidad,
                precio_unitario=producto.precio,
                color=request.form.get('color', 'natural'),
                material=request.form.get('material', 'Algodón Orgánico'),
                paquete=request.form.get('paquete', 'Basic')
            )
            db.session.add(detalle)
            db.session.commit()

            # Redirigir a confirmación con ID de pedido
            return redirect(url_for('confirmacion', pedido_id=pedido.id))

        except Exception as e:
            db.session.rollback()
            flash(f'Error al procesar el pedido: {str(e)}', 'danger')
            return redirect(url_for('checkout'))

    # Si es GET, mostrar formulario con productos disponibles
    productos = Producto.query.all()
    return render_template('checkout.html', productos=productos)

@app.route('/confirmacion/<int:pedido_id>')
def confirmacion(pedido_id):
    pedido = Pedido.query.get_or_404(pedido_id)
    return render_template('confirmacion.html', pedido=pedido)
    
@app.route('/procesar-pago', methods=['POST'])
def procesar_pago():
    try:
        # 1. Procesar los datos del formulario
        datos_pago = {
            'nombre': request.form.get('nombre'),
            'email': request.form.get('email'),
            # Agrega todos los campos necesarios
        }
        
        # 2. Guardar en la base de datos (ejemplo con SQLite)
        pedido = Pedido(
            cliente_id=obtener_cliente_id(datos_pago['email']),
            total=calcular_total(),
            estado='completado'
        )
        db.session.add(pedido)
        db.session.commit()
        
        # 3. Redirigir a confirmación con el ID del pedido
        return redirect(url_for('confirmacion', pedido_id=pedido.id))
        
    except Exception as e:
        db.session.rollback()
        flash('Error al procesar el pago: ' + str(e), 'danger')
        return redirect(url_for('checkout'))

@app.route('/descargar-factura/<int:pedido_id>')
def descargar_factura(pedido_id):
    try:
        # Obtener datos del pedido
        pedido = Pedido.query.get_or_404(pedido_id)
        cliente = Cliente.query.get_or_404(pedido.cliente_id)
        detalles = DetallePedido.query.filter_by(pedido_id=pedido_id).all()
        
        # Validar que existan detalles
        if not detalles:
            return "Error: El pedido no tiene productos", 400

        # Crear PDF
        pdf = FPDF()
        pdf.add_page()
        pdf.set_font("Arial", size=12)
        
        # Encabezado
        pdf.set_font("Arial", 'B', 16)
        pdf.cell(200, 10, txt="MovaSmart", ln=1, align='C')
        pdf.set_font("Arial", size=12)
        pdf.cell(200, 10, txt="Factura de Compra", ln=1, align='C')
        pdf.ln(10)
        
        # Información del cliente
        pdf.set_font("Arial", 'B', 12)
        pdf.cell(200, 10, txt="Datos del Cliente:", ln=1)
        pdf.set_font("Arial", size=12)
        pdf.cell(200, 10, txt=f"Nombre: {cliente.nombre}", ln=1)
        pdf.cell(200, 10, txt=f"Correo: {cliente.correo}", ln=1)
        pdf.cell(200, 10, txt=f"Teléfono: {cliente.telefono}", ln=1)
        pdf.ln(10)
        
        # Detalles factura
        pdf.cell(200, 10, txt=f"N° Factura: {pedido.id}", ln=1)
        pdf.cell(200, 10, txt=f"Fecha: {pedido.fecha_pedido.strftime('%d/%m/%Y %H:%M')}", ln=1)
        pdf.ln(10)
        
        # Tabla de productos
        pdf.set_font("Arial", 'B', 12)
        pdf.cell(100, 10, txt="Producto", border=1)
        pdf.cell(30, 10, txt="Cantidad", border=1)
        pdf.cell(30, 10, txt="Precio", border=1)
        pdf.cell(30, 10, txt="Total", border=1, ln=1)
        
        pdf.set_font("Arial", size=12)
        for detalle in detalles:
            producto = Producto.query.get_or_404(detalle.producto_id)
            pdf.cell(100, 10, txt=producto.nombre, border=1)
            pdf.cell(30, 10, txt=str(detalle.cantidad), border=1)
            pdf.cell(30, 10, txt=f"${detalle.precio_unitario:.2f}", border=1)
            pdf.cell(30, 10, txt=f"${detalle.precio_unitario * detalle.cantidad:.2f}", border=1, ln=1)
        
        # Total
        pdf.set_font("Arial", 'B', 12)
        pdf.cell(160, 10, txt="TOTAL:", border=1)
        pdf.cell(30, 10, txt=f"${pedido.total:.2f}", border=1, ln=1)
        pdf.ln(20)
        
        # Pie de página
        pdf.set_font("Arial", 'I', 10)
        pdf.cell(200, 10, txt="¡Gracias por su compra!", ln=1, align='C')
        pdf.cell(200, 10, txt="MovaSmart - Todos los derechos reservados", ln=1, align='C')

        # Opción 1: Usando send_file (recomendado)
        pdf_bytes = pdf.output(dest='S')
        return send_file(
            io.BytesIO(pdf_bytes),
            mimetype='application/pdf',
            as_attachment=True,
            download_name=f'factura_{pedido.id}.pdf'
        )

        # Opción 2: Alternativa con make_response
        # response = make_response(pdf.output(dest='S'))
        # response.headers['Content-Type'] = 'application/pdf'
        # response.headers['Content-Disposition'] = f'attachment; filename=factura_{pedido.id}.pdf'
        # return response

    except Exception as e:
        # Log del error para debug
        print(f"Error al generar factura: {str(e)}")
        return f"Error al generar la factura: {str(e)}", 500

if __name__ == '__main__':
    app.run(debug=True)
