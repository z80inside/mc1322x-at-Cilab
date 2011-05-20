".vimrc (Ricardo Cañuelo Navarro)

"
set viminfo='100,n~/.viminfo
" Permitir opciones y funciones no compatibles con el vi original
set nocompatible
" Mantener cargado un buffer cuando cambiemos a otro
set hidden
" Mostrar los números de línea a la izquierda
" set nu
" Configuración de la tecla de retroceso
set bs=2
" Usar el ratón desde cualquier modo
set mouse=a
" Tabulaciones de 8 caracteres
set tabstop=8
" Auto-indentación de 8 caracteres
set shiftwidth=8
" Fondo oscuro
set background=dark
" Usar por defecto el registro "*
set clipboard="unnamed"
" Auto-indentación e indentación estilo C
"set cindent
set ai
" Codificación por defecto del terminal y del texto
set tenc=utf-8
set enc=utf-8
" Mostrar el paréntesis/llave/corchete emparejado con el del cursor
set showmatch
" Opciones para el modo gráfico
"set guioptions=T
"set guioptions=m
" Mostrar abajo la línea y número de columna donde está el cursor
"set ruler
" Mostrar siempre la línea de estado
set laststatus=2
" Formato de la línea de estado
set statusline=%F%m%r%h%w\ [%{&ff}]\ [%{&encoding}]\ [%l,%v]\ [%p%%]
" Búsqueda incremental
set incsearch

" Esquema de colores para el modo gráfico
if has('gui_running')
	colorscheme wombat
	set guioptions=aegirLc
	"set guicursor=n-v-c:ver60-Cursor/Cursor,ve:ver35-Cursor,o:hor50-Cursor,i-ci:ver25-Cursor/lCursor,r-cr:hor20-Cursor/lCursor,sm:block-Cursor-blinkwait175-blinkoff150-blinkon175
endif

" Resaltado de sintaxis
syntax on

"highlight Comment gui=italic

" Codificaciones por defecto de los archivos
set fileencodings=utf-8,latin1
" Desactivar los beeps y la visual bell
set vb t_vb=
" Desactivar Line wrapping
set nowrap
" Ancho del texto
set textwidth=80
" Usar tabuladores reales
set noexpandtab
" Fuente para el modo gráfico
set guifont=Inconsolata\ 14

" Mappings
" Desmarcar las palabras encontradas cuando volvamos al modo insert
nnoremap i :noh<CR>i
" Moverse a la pestaña anterior
map <C-Left> :tabp<CR>
" Moverse a la pestaña siguiente
map <C-Right> :tabn<CR>
" Copiar al registro "*
map <C-y> "*y
" Pegar desde el registro "*
map <C-p> "*p
" NERD_Tree
map <F2> :NERDTreeToggle<CR>

