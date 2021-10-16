"""added is_completed column

Revision ID: aece1d95d5fb
Revises: 
Create Date: 2021-10-16 14:46:02.713463

"""
from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision = 'aece1d95d5fb'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.add_column('goals', sa.Column('is_completed', sa.Boolean(), server_default='f', nullable=False))
    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.drop_column('goals', 'is_completed')
    # ### end Alembic commands ###
